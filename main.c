#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

enum Modes { MAIN_MENU, SETTINGS, GAME };

struct Settings {
  int screen_width;
  int screen_height;
  bool should_close;
  int focus;
  int main_title_fsz;
  int fsz;
  int title_position;
  int menu_button_position;
  int buttons_count;
  int position_step_y;
  enum Modes mode;
  float background_scrool_speed;
  bool is_paused;
} settings;

struct PlayerSettings {
  Rectangle position;
  float acceleration;
  float acceleration_speed;
  float decceleration_speed;
  float max_acceleration;
  int health;
} player;

struct MainMenuButton {
  char *text;
  int x_pos;
  int y_pos;
  bool is_focused;
};

struct Background {
  char *path;
  Texture2D texture;
};

int get_text_position_x(int screen_width, char text[], int font_size) {
  return (screen_width - MeasureText(text, font_size)) / 2;
};

int get_text_position_y(int screen_height, int step) {
  return screen_height / 16 * step;
};

void process_key_main_menu(int key, struct Settings *settings) {
  switch (key) {
  case KEY_DOWN:
    if (settings->focus == 2) {
      settings->focus = 0;
      break;
    };
    settings->focus = settings->focus + 1;
    break;
  case KEY_UP:
    if (settings->focus == 0) {
      settings->focus = 2;
      break;
    }
    settings->focus = settings->focus - 1;
    break;
  case KEY_ENTER:
    if (settings->focus == 2) {
      settings->should_close = true;
      break;
    };
    if (settings->focus == 0) {
      settings->mode = GAME;
      break;
    };
    break;
    break;
  default:
    break;
  };
};

void process_key_main_game(int key, struct PlayerSettings *player, int width) {
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

char buttons_text[3][10] = {"Start", "Settings", "Exit"};
struct MainMenuButton buttons[3];

struct Background backgrounds[4] = {
    {.path = "./assets/2D Space Parallax "
             "Backgrounds/Purple/T_PurpleBackground_Version1_Layer1.png"},
    {.path = "./assets/2D Space Parallax "
             "Backgrounds/Purple/T_PurpleBackground_Version1_Layer2.png"},
    {.path = "./assets/2D Space Parallax "
             "Backgrounds/Purple/T_PurpleBackground_Version1_Layer3.png"},
    {.path = "./assets/2D Space Parallax "
             "Backgrounds/Purple/T_PurpleBackground_Version1_Layer4.png"}};

char title[] = "Space Invaders";
char main_menu_title[] = "Hello, Space Invaders";

int main(void) {
  settings.screen_width = 1024;
  settings.screen_height = 800;
  settings.buttons_count = 3;
  settings.focus = 0;
  settings.fsz = 20;
  settings.main_title_fsz = 40;
  settings.title_position = 5;
  settings.menu_button_position = 8;
  settings.should_close = false;
  settings.position_step_y = settings.screen_height / 16;
  settings.mode = MAIN_MENU;
  settings.background_scrool_speed = 0.04f;
  settings.is_paused = false;

  float background_scroll_y = 0.0f;
  InitWindow(settings.screen_width, settings.screen_height, title);

  for (int i = 0; i < 4; i++) {
    backgrounds[0].texture = LoadTexture(backgrounds[0].path);
  };
  Texture2D spaceship_full_health =
      LoadTexture("./assets/Foozle_2DS0011_Void_MainShip/Main Ship/Main Ship - "
                  "Bases/PNGs/Main Ship - Base - Full health.png");
  float background_scale =
      (float)settings.screen_width / backgrounds[0].texture.width;
  // Player rectangles and vector
  Vector2 origin = {spaceship_full_health.width / 2.0f,
                    spaceship_full_health.height / 2.0f};
  Rectangle sourceRec = {0, 0, spaceship_full_health.width,
                         spaceship_full_health.height};
  // Calculate position and focus for menu buttons for different screen sizes
  player.position.x = (float)settings.screen_width / 2;
  player.position.y = 700;
  player.position.width = spaceship_full_health.width;
  player.position.height = spaceship_full_health.height;
  player.health = 100;
  player.acceleration = 0.0f;
  player.acceleration_speed = 100.0f;
  player.decceleration_speed = 120.0f;
  player.max_acceleration = 170.0f;

  for (int i = 0; i < settings.buttons_count; i++) {
    buttons[i].text = buttons_text[i];
    buttons[i].x_pos = get_text_position_x(settings.screen_width,
                                           buttons_text[i], settings.fsz);
    buttons[i].y_pos = get_text_position_y(
        settings.screen_height, settings.menu_button_position + 1 + i);
    if (!i) {
      buttons[i].is_focused = true;
    } else {
      buttons[i].is_focused = false;
    };
  };
  //

  int title_position_x = get_text_position_x(
      settings.screen_width, main_menu_title, settings.main_title_fsz);
  int title_position_y =
      get_text_position_y(settings.screen_height, settings.title_position);

  while (!settings.should_close && !WindowShouldClose()) {
    if (!settings.is_paused) background_scroll_y +=
        settings.background_scrool_speed; // All background assets are the same
                                          // in size;
    if (background_scroll_y >=
        backgrounds[0].texture.height * background_scale) {
      background_scroll_y = 0.0f;
    }
    BeginDrawing();

    // Draw a background
    ClearBackground(BLACK);
    for (int i = 0; i < 4; i++) {
      DrawTextureEx(backgrounds[i].texture, (Vector2){0, background_scroll_y},
                    0.0f, background_scale, WHITE);
      DrawTextureEx(
          backgrounds[i].texture,
          (Vector2){0, background_scroll_y -
                           backgrounds[0].texture.height * background_scale},
          0.0f, background_scale, WHITE);
    };
    if (settings.is_paused)
      DrawText("|| PAUSE", 300, 60, 15, RAYWHITE);
    // end of drawing background

    // MAIN MENU
    if (settings.mode == MAIN_MENU) {
      DrawText(main_menu_title, title_position_x, title_position_y,
               settings.main_title_fsz, RAYWHITE);
      for (int i = 0; i < 3; i++) {
        DrawText(buttons[i].text, buttons[i].x_pos, buttons[i].y_pos,
                 settings.fsz, i == settings.focus ? ORANGE : RAYWHITE);
      };
      if (IsKeyPressed(KEY_DOWN))
        process_key_main_menu(KEY_DOWN, &settings);
      if (IsKeyPressed(KEY_UP))
        process_key_main_menu(KEY_UP, &settings);
      if (IsKeyPressed(KEY_ENTER))
        process_key_main_menu(KEY_ENTER, &settings);
    };
    // END OF MAIN MENU

    // ACTUAL GAME
    if (settings.mode == GAME) {
      if (IsKeyPressed(KEY_SPACE))
        settings.is_paused = !settings.is_paused;
      if (!settings.is_paused) player.position.x += player.acceleration * GetFrameTime();
      if (player.position.x < 0)
        player.position.x = settings.screen_width;
      if (player.position.x > settings.screen_width)
        player.position.x = 0;

      DrawText("Press Backspace to Leave", 20, 20, 10, RAYWHITE);
      if (IsKeyReleased(KEY_BACKSPACE)) {
        settings.mode = MAIN_MENU;
      };
      DrawText(
          TextFormat("X = %f, Y = %f", player.position.x, player.position.y),
          settings.screen_width - 300, 20, 15, RAYWHITE);
      DrawText(TextFormat("Acc = %f", player.acceleration),
               settings.screen_width - 300, 40, 15, RAYWHITE);
      DrawTexturePro(spaceship_full_health, sourceRec, player.position, origin,
                     0.0f, WHITE);
      DrawRectangle(20, settings.screen_height - 50, 100, 20, GREEN);
      if (IsKeyDown(KEY_LEFT) && !settings.is_paused) {
        process_key_main_game(KEY_LEFT, &player, settings.screen_width);
      } else {
        if (player.acceleration < 0 && !settings.is_paused) {
          player.acceleration += player.decceleration_speed * GetFrameTime();
        };
      };
      if (IsKeyDown(KEY_RIGHT)&& !settings.is_paused) {
        process_key_main_game(KEY_RIGHT, &player, settings.screen_width);
      } else {
        if (player.acceleration > 0 && !settings.is_paused) {
          player.acceleration -= player.decceleration_speed * GetFrameTime();
        };
      };
    };
    // END OF ACTUAL GAME
    EndDrawing();
  }
  for (int i = 0; i < 4; i++) {
    UnloadTexture(backgrounds[i].texture);
  };
  UnloadTexture(spaceship_full_health);
  CloseWindow();
  return 0;
}
