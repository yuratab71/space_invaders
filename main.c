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

const char *name_enemy_movement(enum EnemyMovement enemy_movement) {
  switch (enemy_movement) {
  case LEFT:
    return "LEFT";
  case RIGHT:
    return "RIGHT";
  case DOWN:
    return "DOWN";
  default:
    return "UNKNOWN";
  };
};

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
  settings.screen_width = 1024;
  settings.screen_height = 720;
  settings.should_close = false;
  settings.mode = MENU;
  settings.is_paused = false;

  InitWindow(settings.screen_width, settings.screen_height, title);
  SetWindowState(FLAG_VSYNC_HINT);

  // Background initialization
  background.texture = LoadTexture(
      "./assets/PixelSpaceRage/PixelBackgroundSeamlessVertically.png");
  background.y_pos = 0.0f;
  background.scroll_speed = 0.5f;
  background.scale =
      GetBackgroundScale(settings.screen_width, background.texture.width);
  // end of background initialization

  // Menu initialization
  MenuInit(&menu, &settings);
  ButtonsInit(buttons, &settings, &menu);
  // End of menu initialization

  Texture2D spaceship_idle = LoadTexture(
      "./assets/PixelSpaceRage/128px/PlayerBlue_Frame_01_png_processed.png");
  Texture2D spaceship_turn_right_1 =
      LoadTexture("./assets/PixelSpaceRage/128px/"
                  "PlayerBlue_Frame_02_png_processed_flipped.png");
  Texture2D spaceship_turn_right_2 =
      LoadTexture("./assets/PixelSpaceRage/128px/"
                  "PlayerBlue_Frame_03_png_processed_flipped.png");
  Texture2D spaceship_turn_left_1 = LoadTexture(
      "./assets/PixelSpaceRage/128px/PlayerBlue_Frame_02_png_processed.png");
  Texture2D spaceship_turn_left_2 = LoadTexture(
      "./assets/PixelSpaceRage/128px/PlayerBlue_Frame_03_png_processed.png");
  Texture2D projectile = LoadTexture(
      "./assets/PixelSpaceRage/128px/Laser_Large_png_processed.png");

  // Player rectangles and vector
  Vector2 origin = {spaceship_idle.width / 2.0f, spaceship_idle.height / 2.0f};
  Rectangle sourceRec = {0, 0, spaceship_idle.width, spaceship_idle.height};

  player.position.x = (float)settings.screen_width / 2;
  player.position.y = settings.screen_height - 70;
  player.position.width = spaceship_idle.width;
  player.position.height = spaceship_idle.height;
  player.health = 100;
  player.acceleration = 0.0f;
  player.acceleration_speed = 100.0f;
  player.decceleration_speed = 120.0f;
  player.max_acceleration = 270.0f;
  player.can_shoot = true;
  player.bullet.pos.x = player.position.x;
  player.bullet.pos.y = 700;
  player.bullet.collider.x = player.bullet.pos.x;
  player.bullet.collider.y = player.bullet.pos.y;
  player.bullet.collider.width = projectile.width;
  player.bullet.collider.height = projectile.height;

  playerArea.y = player.position.y - 30;
  playerArea.x = player.position.x - 15;
  playerArea.height = spaceship_idle.height;
  playerArea.width = (float)spaceship_idle.width / 2;

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
      CalculateBackgroundPosition(&background);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawBackground(&background);
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

      if (IsKeyPressed(KEY_P))
        settings.is_paused = !settings.is_paused;
      if (!settings.is_paused) {
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

      DrawText("Press Backspace to Leave", 20, 20, 10, RAYWHITE);
      if (IsKeyReleased(KEY_BACKSPACE)) {
        settings.mode = MENU;
      };

      // Draw Metrics, just for development
      DrawText(
          TextFormat("X = %f, Y = %f", player.position.x, player.position.y),
          settings.screen_width - 300, 20, 15, RAYWHITE);
      DrawText(TextFormat("Acc = %f", player.acceleration),
               settings.screen_width - 300, 40, 15, RAYWHITE);
      DrawText(TextFormat("Move counter = %d", enemy_move_counter),
               settings.screen_width - 300, 60, 15, RAYWHITE);
      DrawText(TextFormat("Direction = %s",
                          name_enemy_movement(enemy_move_direction)),
               settings.screen_width - 300, 80, 15, RAYWHITE);
      DrawText(TextFormat("Direction = %s",
                          name_enemy_movement(enemy_move_dir_prev)),
               settings.screen_width - 300, 100, 15, RAYWHITE);
      DrawText(TextFormat("Bullet x = %f, y = %f", player.bullet.pos.x,
                          player.bullet.pos.y),
               settings.screen_width - 300, 120, 15, RAYWHITE);
      //

      // Draw Player
      DrawRectangle(playerArea.x, playerArea.y, playerArea.width,
                    playerArea.height, GREEN);
      if (player.acceleration > 70.0f || player.acceleration < -70.0f) {
        DrawTexturePro(player.acceleration > 0 ? spaceship_turn_right_2
                                               : spaceship_turn_left_2,
                       sourceRec, player.position, origin, 0.0f, WHITE);
      } else if (player.acceleration > 30.0f || player.acceleration < -30.0f) {
        DrawTexturePro(player.acceleration > 0 ? spaceship_turn_right_1
                                               : spaceship_turn_left_1,
                       sourceRec, player.position, origin, 0.0f, WHITE);
      } else {
        DrawTexturePro(spaceship_idle, sourceRec, player.position, origin, 0.0f,
                       WHITE);
      };
      // end of draw player
      //
      // Draw Projectiles
      if (!settings.is_paused) {
        GameCalculateBullets(&player, delta);
      };

      if (IsKeyPressed(KEY_SPACE) && !settings.is_paused && player.can_shoot) {
        GameProcessShooting(&player);
      };

      if (player.bullet.pos.y < player.position.y) {
        DrawTextureEx(projectile, player.bullet.pos, 0.0f, 1.0f, WHITE);
      };
      //

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
      //
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
    };
    // END OF ACTUAL GAME
    EndDrawing();
  };
  UnloadTexture(background.texture);
  UnloadTexture(spaceship_idle);
  UnloadTexture(spaceship_turn_left_1);
  UnloadTexture(spaceship_turn_left_2);
  UnloadTexture(spaceship_turn_right_1);
  UnloadTexture(spaceship_turn_right_2);
  UnloadTexture(enemy_green_txtr);
  UnloadTexture(enemy_red_small_txtr);
  UnloadTexture(enemy_red_txtr);
  UnloadTexture(enemy_teal_txtr);
  UnloadTexture(projectile);
  CloseWindow();
  return 0;
}
