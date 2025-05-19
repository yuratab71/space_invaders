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
}
#endif

enum EnemyMovement { LEFT, RIGHT, DOWN };

PlayerSettings player;
Rectangle playerArea;
Enemy enemies[6][5];
GlobalSettings settings;
BackgroundSettings background;
MenuSettings menu;
Btn buttons[2];
Projectile enemy_projectile;
float enemy_can_shoot_timer;
bool enemy_can_shoot;
Enemy wandering_enemy;
char title[] = "Space Invaders";
char main_menu_title[] = "Hello, Space Invaders";

int main(void) {
  if (NvOptimusEnablement && AmdPowerXpressRequestHighPerformance) {
    printf("[GAME]: Use dedicated GPU enabled \n");
  };

  SettingsInit(&settings);
  InitWindow(settings.screen_width, settings.screen_height, title);
  SetWindowState(FLAG_VSYNC_HINT);
  BackgroundLoadTexture(&background);
  BackgroundInit(&background, &settings);
  MenuInit(&menu, &settings);
  MenuInitButtons(buttons, &settings, &menu);
  GameLoadPlayerTextures(&player);
  GameInitPlayer(&player, &settings);

  playerArea.y = player.position.y - 30;
  playerArea.x = player.position.x - 15;
  playerArea.height = player.idle.height;
  playerArea.width = (float)player.idle.width / 2;

  Vector2 origin = {player.idle.width / 2.0f, player.idle.height / 2.0f};
  Rectangle sourceRec = {0, 0, player.idle.width, player.idle.height};

  Texture2D enemy_red_txtr = LoadTexture(
      "./assets/PixelSpaceRage/128px/Enemy01_Red_Frame_2_png_processed.png");
  Texture2D enemy_green_txtr = LoadTexture(
      "./assets/PixelSpaceRage/128px/Enemy01_Green_Frame_1_png_processed.png");
  Texture2D enemy_teal_txtr = LoadTexture(
      "./assets/PixelSpaceRage/128px/Enemy01_Teal_Frame_1_png_processed.png");
  Texture2D enemy_red_small_txtr = LoadTexture(
      "./assets/PixelSpaceRage/128px/Enemy02Red_Frame_1_png_processed.png");

  // Enemy init
  float enemy_move_timer = 120.0f;
  int enemy_move_counter = 2;
  int enemy_move_dir_prev = LEFT;
  int enemy_move_direction = LEFT;
  float enemy_move_step = 40.0f;
  int enemy_start_pos_x = (int)settings.screen_width / 2 - 2 * 80;
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 5; j++) {
      enemies[i][j].collider.x = enemy_start_pos_x + j * 80 + 15.0f;
      enemies[i][j].collider.y = 50 + i * 50 + 10.0f;
      enemies[i][j].pos.x = enemy_start_pos_x + j * 80;
      enemies[i][j].pos.y = 50 + i * 50;

      enemies[i][j].collider.width = 35;
      enemies[i][j].collider.height = 40;

      enemies[i][j].bullet.x = 0;
      enemies[i][j].bullet.y = 0;
      enemies[i][j].bullet.height = 20;
      enemies[i][j].bullet.width = 20;

      enemies[i][j].is_alive = true;
    };
  };

  wandering_enemy.pos.x = -50.0f;
  wandering_enemy.pos.y = 50.0f;
  wandering_enemy.collider.x = wandering_enemy.pos.x;
  wandering_enemy.collider.y = wandering_enemy.pos.y + 20.0f;
  wandering_enemy.collider.width = 30;
  wandering_enemy.collider.height = 30;
  bool is_wander_enemy = false;
  float is_wander_enemy_timer = 0.0f;

  enemy_projectile.pos = GameGetRandomEnemyPosition(enemies);
  enemy_projectile.collider.x = enemy_projectile.pos.x;
  enemy_projectile.collider.y = enemy_projectile.pos.y;
  enemy_projectile.collider.width = 15;
  enemy_projectile.collider.height = 15;
  enemy_can_shoot = false;
  enemy_can_shoot_timer = 0.0f;

  Texture2D enemy_projectile_texture = LoadTexture(
      "./assets/PixelSpaceRage/128px/Plasma_Large_png_processed.png");

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

        if (!is_wander_enemy) {
          is_wander_enemy_timer += 100.0f * delta;
        };
        if (!is_wander_enemy && is_wander_enemy_timer >= 1200.0f) {
          wandering_enemy.pos.x = -50.0f;
          is_wander_enemy = true;
          is_wander_enemy_timer = 0.0f;
        }
        if (is_wander_enemy) {
          if (wandering_enemy.pos.x >= settings.screen_width) {
            is_wander_enemy = false;
          };
          wandering_enemy.pos.x += 100.0f * delta;
          wandering_enemy.collider.x = wandering_enemy.pos.x + 15.0f;
        };
        if (enemy_can_shoot) {
          enemy_can_shoot_timer += 400.0f * delta;
        };
        if (enemy_can_shoot && enemy_can_shoot_timer >= 500.0f) {
          enemy_projectile.pos = GameGetRandomEnemyPosition(enemies);
          enemy_projectile.collider.x = enemy_projectile.pos.x - 12.0f;
          enemy_projectile.collider.y = enemy_projectile.pos.y - 12.0f;
          enemy_can_shoot = false;
          enemy_can_shoot_timer = 0.0f;
        } else {
          if (enemy_projectile.pos.y > settings.screen_height)
            enemy_can_shoot = true;
          enemy_projectile.pos.y += 300.0f * delta;
          enemy_projectile.collider.y = enemy_projectile.pos.y - 15.0f;
        };
        if (CheckCollisionRecs(playerArea, enemy_projectile.collider)) {
          settings.is_paused = true;
        };
        if (CheckCollisionRecs(player.bullet.collider,
                               wandering_enemy.collider)) {
          is_wander_enemy = false;
          is_wander_enemy_timer = 0.0f;
        };

        for (int i = 0; i < 6; i++) {
          for (int j = 0; j < 5; j++) {
            GameProcessCollisionBulletOnEnemy(&player, &enemies[i][j]);
          };
        };
        playerArea.x = player.position.x - 15;
        enemy_move_timer -= 1.0f;
        if (enemy_move_timer < 1.0f) {
          if (enemy_move_counter == 0 &&
              (enemy_move_direction == LEFT || enemy_move_direction == RIGHT)) {
            enemy_move_dir_prev = enemy_move_direction;
            enemy_move_direction = DOWN;
          }
          switch (enemy_move_direction) {
          case LEFT:
            for (int i = 0; i < 6; i++) {
              for (int j = 0; j < 5; j++) {
                enemies[i][j].collider.x -= enemy_move_step;
                enemies[i][j].pos.x -= enemy_move_step;
              };
            };
            enemy_move_counter -= 1;
            enemy_move_timer = 120.0f;
            break;
          case RIGHT:
            for (int i = 0; i < 6; i++) {
              for (int j = 0; j < 5; j++) {
                enemies[i][j].collider.x += enemy_move_step;
                enemies[i][j].pos.x += enemy_move_step;
              };
            };
            enemy_move_counter -= 1;
            enemy_move_timer = 120.0f;
            break;
          case DOWN:
            for (int i = 0; i < 6; i++) {
              for (int j = 0; j < 5; j++) {
                enemies[i][j].collider.y += enemy_move_step;
                enemies[i][j].pos.y += enemy_move_step;
              };
            };
            enemy_move_counter = 5;
            if (enemy_move_dir_prev == LEFT)
              enemy_move_direction = RIGHT;
            if (enemy_move_dir_prev == RIGHT)
              enemy_move_direction = LEFT;
            enemy_move_timer = 120.0f;
            break;
          default:
            break;
          };
        };
      };
      // DRAWING SECTION
      //
      DrawRectangle(playerArea.x, playerArea.y, playerArea.width,
                    playerArea.height, GREEN);
      GameDrawPlayer(&player, sourceRec, origin);
      GameDrawPlayerBullet(&player);

      DrawText(TextFormat("x = %f", enemy_projectile.pos.x), 800, 100, 20,
               RAYWHITE);
      DrawText(TextFormat("y = %f", enemy_projectile.pos.y), 800, 150, 20,
               RAYWHITE);
      DrawText(TextFormat("can shoot = %d", enemy_can_shoot), 800, 170, 20,
               RAYWHITE);
      DrawText(TextFormat("Timer = %f", enemy_can_shoot_timer), 800, 190, 20,
               RAYWHITE);
      DrawText(TextFormat("wander? = %d", is_wander_enemy), 800, 210, 20,
               RAYWHITE);
      DrawText(TextFormat("Wander timer = %f", is_wander_enemy_timer), 800, 250,
               20, RAYWHITE);
      DrawText(TextFormat("Wx = %f", wandering_enemy.collider.x), 800, 270, 20,
               RAYWHITE);
      DrawText(TextFormat("Wy = %f", wandering_enemy.collider.y), 800, 290, 20,
               RAYWHITE);
      if (!enemy_can_shoot) {
        DrawTextureEx(enemy_projectile_texture, enemy_projectile.pos, 180.0f,
                      1.0f, WHITE);
        DrawRectangle(enemy_projectile.collider.x, enemy_projectile.collider.y,
                      enemy_projectile.collider.width,
                      enemy_projectile.collider.height, RED);
      };
      if (is_wander_enemy) {
        DrawTextureEx(enemy_green_txtr, wandering_enemy.pos, 0.0f, 1.0f, WHITE);
        DrawRectangle(wandering_enemy.collider.x, wandering_enemy.collider.y,
                      wandering_enemy.collider.width,
                      wandering_enemy.collider.height, RAYWHITE);
      };
      // Draw Enemies
      GameDrawEnemies(enemies, &enemy_red_txtr);
      // End of Draw enemies
      //
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
  UnloadTexture(enemy_green_txtr);
  UnloadTexture(enemy_red_small_txtr);
  UnloadTexture(enemy_red_txtr);
  UnloadTexture(enemy_teal_txtr);
  UnloadTexture(enemy_projectile_texture);
  CloseWindow();
  return 0;
}
