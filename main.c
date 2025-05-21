#include "background.h"
#include "game.h"
#include "global_settings.h"
#include "main_menu.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#ifdef __cplusplus

#endif

int enemy_on_x;
int enemy_on_y;

PlayerSettings player;
EnemiesSettings e_settings;
EnemyTextures e_textures;
GlobalSettings settings;
BackgroundSettings background;
MenuSettings menu;
Btn buttons[2];
Projectile enemy_projectile;
Enemy wandering_enemy;
char title[] = "Space Invaders";
char main_menu_title[] = "Hello, Space Invaders";

int main(void) {
  if (NvOptimusEnablement && AmdPowerXpressRequestHighPerformance) {
    printf("[GAME]: Use dedicated GPU enabled \n");
  };
  enemy_on_x = ENEMY_ON_X;
  enemy_on_y = ENEMY_ON_Y;
  Enemy enemies[enemy_on_x][enemy_on_y];

  SettingsInit(&settings);
  InitWindow(settings.screen_width, settings.screen_height, title);
  SetWindowState(FLAG_VSYNC_HINT);
  BackgroundLoadTexture(&background);
  BackgroundInit(&background, &settings);
  MenuInit(&menu, &settings);
  MenuInitButtons(buttons, &settings, &menu);
  GameLoadPlayerTextures(&player);
  GameInitPlayer(&player, &settings);

  Vector2 origin = {player.idle.width / 2.0f, player.idle.height / 2.0f};
  Rectangle sourceRec = {0, 0, player.idle.width, player.idle.height};

  // Enemy init
  GameInitEnemies(&e_settings, enemy_on_x, enemy_on_y, enemies,
                  &wandering_enemy, &enemy_projectile,
                  (float)settings.screen_width / 2 - 2 * 80);
  GameLoadEnemyTextures(&e_textures);

  while (!settings.should_close && !WindowShouldClose()) {
    float delta = GetFrameTime();

    if (!settings.is_paused)
      BackgrounfCalculatePosition(&background);

    BeginDrawing();
    ClearBackground(BLACK);
    BackgroundDrawSelf(&background);
    // MAIN MENU
    if (settings.mode == MENU) {
      MenuDrawTitle(&menu);
      MenuDrawButtons(&menu, buttons);
      if (IsKeyPressed(KEY_DOWN))
        MenuProcessKey(KEY_DOWN, &menu, &settings);
      if (IsKeyPressed(KEY_UP))
        MenuProcessKey(KEY_UP, &menu, &settings);
      if (IsKeyPressed(KEY_ENTER))
        MenuProcessKey(KEY_ENTER, &menu, &settings);
    };
    // END OF MAIN MENU

    // ACTUAL GAME
    if (settings.mode == GAME) {
      if (settings.is_paused)
        DrawText("|| PAUSE", 300, 60, 15, RAYWHITE);
      if (!settings.is_paused) {
        GameCalculateBullets(&player, delta);
        GameCalculatePlayer(&player, delta, &settings);

        e_settings.move_timer -= 60.0f * delta;
        player.collider.x = player.position.x - 15;
        
        if (!e_settings.is_wander) {
          e_settings.is_wtimer += 100.0f * delta;
        } else {
          if (wandering_enemy.pos.x >= settings.screen_width) {
            e_settings.is_wander = false;
          };
          wandering_enemy.pos.x += 100.0f * delta;
          wandering_enemy.collider.x = wandering_enemy.pos.x + 15.0f;
        };
        if (!e_settings.is_wander && e_settings.is_wtimer > 1200.0f) {
          wandering_enemy.pos.x = -50.0f;
          e_settings.is_wander = true;
          e_settings.is_wtimer = 0.0f;
        };

        if (e_settings.can_shoot) {
          e_settings.shoot_timer += 400.0f * delta;
        };
        if (e_settings.can_shoot && e_settings.shoot_timer >= 500.0f) {
          enemy_projectile.pos =
              GameGetRandomEnemyPosition(enemy_on_x, enemy_on_y, enemies);
          enemy_projectile.collider.x = enemy_projectile.pos.x - 12.0f;
          enemy_projectile.collider.y = enemy_projectile.pos.y - 12.0f;
          e_settings.can_shoot = false;
          e_settings.shoot_timer = 0.0f;
        } else {
          if (enemy_projectile.pos.y > settings.screen_height)
            e_settings.can_shoot = true;
          enemy_projectile.pos.y += 300.0f * delta;
          enemy_projectile.collider.y = enemy_projectile.pos.y - 15.0f;
        };
        if (CheckCollisionRecs(player.collider, enemy_projectile.collider)) {
          //          settings.is_paused = true;
        };
        if (CheckCollisionRecs(player.bullet.collider,
                               wandering_enemy.collider)) {
          e_settings.is_wander = false;
          e_settings.is_wtimer = 0.0f;
          player.score += 300;
        };

        for (int i = 0; i < enemy_on_x; i++) {
          for (int j = 0; j < enemy_on_y; j++) {
            GameProcessCollisionBulletOnEnemy(&player, &enemies[i][j]);
          };
        };
        GameProcessEnemyGridMovement(&e_settings, enemy_on_x, enemy_on_y,
                                     enemies);
      };
      // DRAWING SECTION
      //
      DrawRectangle(player.collider.x, player.collider.y, player.collider.width,
                    player.collider.height, GREEN);
      GameDrawPlayer(&player, sourceRec, origin);
      GameDrawPlayerBullet(&player);

      DrawText(TextFormat("x = %f", enemy_projectile.pos.x), 800, 100, 20,
               RAYWHITE);
      DrawText(TextFormat("y = %f", enemy_projectile.pos.y), 800, 150, 20,
               RAYWHITE);
      DrawText(TextFormat("can shoot = %d", e_settings.can_shoot), 800, 170, 20,
               RAYWHITE);
      DrawText(TextFormat("Timer = %f", e_settings.shoot_timer), 800, 190, 20,
               RAYWHITE);
      DrawText(TextFormat("wander? = %d", e_settings.is_wander), 800, 210, 20,
               RAYWHITE);
      DrawText(TextFormat("Wander timer = %f", e_settings.is_wtimer), 800, 250,
               20, RAYWHITE);
      DrawText(TextFormat("Wx = %f", wandering_enemy.collider.x), 800, 270, 20,
               RAYWHITE);
      DrawText(TextFormat("Wy = %f", wandering_enemy.collider.y), 800, 290, 20,
               RAYWHITE);
      DrawText(TextFormat("Dir = %d", e_settings.move_dir), 800, 310, 20,
               RAYWHITE);
      DrawText(TextFormat("Count = %d", e_settings.move_counter), 800, 330, 20,
               RAYWHITE);
      DrawText(TextFormat("Move timer = %f", e_settings.move_timer), 800, 350,
               20, RAYWHITE);

      DrawText(TextFormat("%d", player.score), 50, 50, 30, RAYWHITE);
      if (!e_settings.can_shoot) {
        DrawTextureEx(e_textures.projectile, enemy_projectile.pos, 180.0f, 1.0f,
                      WHITE);
        DrawRectangle(enemy_projectile.collider.x, enemy_projectile.collider.y,
                      enemy_projectile.collider.width,
                      enemy_projectile.collider.height, RED);
      };
      if (e_settings.is_wander) {
        DrawTextureEx(e_textures.green, wandering_enemy.pos, 0.0f, 1.0f, WHITE);
        DrawRectangle(wandering_enemy.collider.x, wandering_enemy.collider.y,
                      wandering_enemy.collider.width,
                      wandering_enemy.collider.height, RAYWHITE);
      };
      GameDrawEnemies(enemy_on_x, enemy_on_y, enemies, &e_textures.red);

      // INPUT SECTION
      if (IsKeyReleased(KEY_BACKSPACE)) {
        settings.mode = MENU;
      };

      if (IsKeyPressed(KEY_SPACE) && !settings.is_paused && player.can_shoot) {
        GameProcessShooting(&player);
      };

      if (IsKeyDown(KEY_LEFT) && !settings.is_paused) {
        GameProcessKeyMovement(KEY_LEFT, &player);
      } else {
        if (player.acceleration < 0.0f && !settings.is_paused) {
          if (player.acceleration > -0.5f) {
            player.acceleration = 0.0f;
          } else {
            player.acceleration += player.decceleration_speed * delta;
          };
        };
      };
      if (IsKeyDown(KEY_RIGHT) && !settings.is_paused) {
        GameProcessKeyMovement(KEY_RIGHT, &player);
      } else {
        if (player.acceleration > 0.0f && !settings.is_paused) {
          if (player.acceleration < 0.5f) {
            player.acceleration = 0.0f;
          } else {
            player.acceleration -= player.decceleration_speed * delta;
          };
        };
      };
      if (IsKeyPressed(KEY_P))
        settings.is_paused = !settings.is_paused;
    };
    // INPUT END
    //
    // END OF ACTUAL GAME
    EndDrawing();
  };
  BackgroundUnloadTexture(&background);
  GameUnloadPlayerTextures(&player);
  GameUnloadEnemyTextures(&e_textures);
  CloseWindow();
  return 0;
};
