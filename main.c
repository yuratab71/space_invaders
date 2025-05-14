#include "background.h"
#include "global_settings.h"
#include "main_menu.h"
#include "raylib.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) unsigned long NvOptimusEnablement = 1;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#ifdef __cplusplus
}
#endif

struct Projectile {
  struct Vector2 pos;
  float acceleration;
};

struct PlayerSettings {
  Rectangle position;
  float acceleration;
  float acceleration_speed;
  float decceleration_speed;
  float max_acceleration;
  struct Projectile bullets[5];
  int health;
} player;

GlobalSettings settings;
BackgroundSettings background;
MenuSettings menu;
Btn buttons[2];
char *btn_text[] = {"Start", "Exit"};

void process_key_main_game(int key, struct PlayerSettings *player) {
  switch (key) {
  case KEY_LEFT:
    if (player->acceleration <= 0.0f - player->max_acceleration)
      break;
    player->acceleration -= player->acceleration_speed * GetFrameTime();
    break;
  case KEY_RIGHT:
    if (player->acceleration >= player->max_acceleration)
      break;
    player->acceleration += player->acceleration_speed * GetFrameTime();
    break;
  default:
    break;
  };
};

char title[] = "Space Invaders";
char main_menu_title[] = "Hello, Space Invaders";

int main(void) {
  settings.screen_width = 1024;
  settings.screen_height = 800;
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
  menu.title_fz = 30;
  menu.title_text = "Hello, Space Invaders!";
  menu.title_pos_y = 400;
  menu.title_pos_x =
      CalculateXPos(menu.title_text, settings.screen_width, menu.title_fz);

  menu.position_step_y = 8;
  menu.focus = 0;
  menu.btn_fsz = 20;

  for (int i = 0; i < 2; i++) {
    buttons[i].text = btn_text[i];
    buttons[i].step = menu.position_step_y + i + 1;
    buttons[i].x_pos =
        CalculateXPos(buttons[i].text, settings.screen_width, menu.btn_fsz);
    buttons[i].y_pos =
        CalculateYPos(settings.screen_height, menu.position_step_y);
    buttons[i].y_pos = 500;
    buttons[i].is_focused = !i;
  };
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
  player.position.y = 700;
  player.position.width = spaceship_idle.width;
  player.position.height = spaceship_idle.height;
  player.health = 100;
  player.acceleration = 0.0f;
  player.acceleration_speed = 100.0f;
  player.decceleration_speed = 120.0f;
  player.max_acceleration = 270.0f;
  for (int i = 0; i < 5; i++) {
    player.bullets[i].acceleration = 0.0f;
    player.bullets[i].pos.x = (float)settings.screen_width / 2;
    player.bullets[i].pos.y = 700;
  };
  int bullet_counter = 5;

  while (!settings.should_close && !WindowShouldClose()) {
    if (!settings.is_paused)
      CalculateBackgroundPosition(&background);

    BeginDrawing();

    // Draw a background
    ClearBackground(BLACK);
    DrawBackground(&background);

    if (settings.is_paused)
      DrawText("|| PAUSE", 300, 60, 15, RAYWHITE);
    // end of drawing background

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
      if (IsKeyPressed(KEY_P))
        settings.is_paused = !settings.is_paused;
      if (!settings.is_paused)
        player.position.x += player.acceleration * GetFrameTime();
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
      //

      // Draw Player
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
      for (int i = 0; i < 5; i++) {
        if (player.bullets[i].pos.y < -settings.screen_height) {
          player.bullets[i].pos.y = player.position.y;
          player.bullets[i].pos.x = player.position.x;
          player.bullets[i].acceleration = 0.0f;
        };
        if (player.bullets[i].pos.y < player.position.y)
          player.bullets[i].acceleration += 15.0f * GetFrameTime();
        player.bullets[i].pos.y -= player.bullets[i].acceleration;
      };
      if (IsKeyPressed(KEY_SPACE) && bullet_counter > 0) {
        player.bullets[bullet_counter - 1].pos.y =
            player.position.y - spaceship_idle.height;
        player.bullets[bullet_counter - 1].pos.x = player.position.x;
        if (bullet_counter <= 5 && bullet_counter > 0)
          bullet_counter -= 1;
      };
      if (IsKeyPressed(KEY_R))
        bullet_counter = 5;
      for (int i = 0; i < 5; i++) {
        if (player.bullets[i].pos.y < player.position.y)
          DrawTextureEx(projectile, player.bullets[i].pos, 0.0f, 1.0f, WHITE);
      };
      DrawText(TextFormat("Ammo %d", bullet_counter),
               settings.screen_width - 60, settings.screen_height - 30, 15,
               RAYWHITE);
      //
      // Health Bar
      DrawRectangle(20, settings.screen_height - 50, 100, 20, GREEN);

      if (IsKeyDown(KEY_LEFT) && !settings.is_paused) {
        process_key_main_game(KEY_LEFT, &player);
      } else {
        if (player.acceleration < 0.0f && !settings.is_paused) {
          if (player.acceleration > -0.5f) {
            player.acceleration = 0.0f;
          } else {
            player.acceleration += player.decceleration_speed * GetFrameTime();
          };
        };
      };
      if (IsKeyDown(KEY_RIGHT) && !settings.is_paused) {
        process_key_main_game(KEY_RIGHT, &player);
      } else {
        if (player.acceleration > 0.0f && !settings.is_paused) {
          if (player.acceleration < 0.5f) {
            player.acceleration = 0.0f;
          } else {
            player.acceleration -= player.decceleration_speed * GetFrameTime();
          };
        };
      };
    };
    // END OF ACTUAL GAME
    EndDrawing();
  }
  UnloadTexture(background.texture);
  UnloadTexture(spaceship_idle);
  UnloadTexture(spaceship_turn_left_1);
  UnloadTexture(spaceship_turn_left_2);
  UnloadTexture(spaceship_turn_right_1);
  UnloadTexture(spaceship_turn_right_2);
  UnloadTexture(projectile);
  CloseWindow();
  return 0;
}
