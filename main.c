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
Enemy enemies[4][4];
GlobalSettings settings;
BackgroundSettings background;
MenuSettings menu;
Btn buttons[2];
char title[] = "Space Invaders";
char main_menu_title[] = "Hello, Space Invaders";

int main(void) {
  if (NvOptimusEnablement && AmdPowerXpressRequestHighPerformance) {
    printf("[GAME]: Use dedicated GPU enabled");
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
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      enemies[i][j].collider.x = enemy_start_pos_x + j * 80;
      enemies[i][j].collider.y = 50 + i * 50;
      enemies[i][j].collider.width = 50;
      enemies[i][j].collider.height = 40;

      enemies[i][j].bullet.x = 0;
      enemies[i][j].bullet.y = 0;
      enemies[i][j].bullet.height = 20;
      enemies[i][j].bullet.width = 20;

      enemies[i][j].is_alive = true;
    };
  };

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
        player.position.x += player.acceleration * delta;
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
            for (int i = 0; i < 4; i++) {
              for (int j = 0; j < 4; j++) {
                enemies[i][j].collider.x -= enemy_move_step;
              };
            };
            enemy_move_counter -= 1;
            enemy_move_timer = 120.0f;
            break;
          case RIGHT:
            for (int i = 0; i < 4; i++) {
              for (int j = 0; j < 4; j++) {
                enemies[i][j].collider.x += enemy_move_step;
              };
            };
            enemy_move_counter -= 1;
            enemy_move_timer = 120.0f;
            break;
          case DOWN:
            for (int i = 0; i < 4; i++) {
              for (int j = 0; j < 4; j++) {
                enemies[i][j].collider.y += enemy_move_step;
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
      if (player.position.x < 0)
        player.position.x = settings.screen_width;
      if (player.position.x > settings.screen_width)
        player.position.x = 0;

      // DRAWING SECTION
      //
      DrawRectangle(playerArea.x, playerArea.y, playerArea.width,
                    playerArea.height, GREEN);
      GameDrawPlayer(&player, sourceRec, origin);
      GameDrawPlayerBullet(&player);

      // Draw Enemies
      for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
          if (enemies[i][j].is_alive) {
            DrawRectangle(enemies[i][j].collider.x, enemies[i][j].collider.y,
                          enemies[i][j].collider.width,
                          enemies[i][j].collider.height, RAYWHITE);
            DrawTextureEx(
                enemy_red_txtr,
                (Vector2){enemies[i][j].collider.x, enemies[i][j].collider.y},
                0.0f, 1.0f, WHITE);
          };
        };
      };
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
  CloseWindow();
  return 0;
}
