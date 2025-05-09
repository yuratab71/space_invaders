#include "raylib.h"
#include <stdbool.h>

enum Modes { MAIN_MENU, SETTINGS, PAUSE, GAME };

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
};
struct Settings settings;

struct MainMenuButton {
  char *text;
  int x_pos;
  int y_pos;
  bool is_focused;
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

char buttons_text[3][10] = {"Start", "Settings", "Exit"};
struct MainMenuButton buttons[3];

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
  settings.background_scrool_speed = 0.0f;

  InitWindow(settings.screen_width, settings.screen_height, title);
  Texture2D background_layer_1 =
      LoadTexture("./assets/Red_Background_Version2_Layer1.png");
  Texture2D background_layer_2 =
      LoadTexture("./assets/Red_Background_Version2_Layer2.png");
  SetTargetFPS(60);
  Texture2D background_layer_3 =
      LoadTexture("./assets/Red_Background_Version2_Layer3.png");
  Texture2D background_layer_4 =
      LoadTexture("./assets/Red_Background_Version2_Layer4.png");

  // Calculate position and focus for menu buttons for different screen sizes
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
    settings.background_scrool_speed -= 0.4f;
    if (settings.background_scrool_speed <= -background_layer_1.width * 2)
      settings.background_scrool_speed = 0;

    BeginDrawing();

    // Draw a background
    ClearBackground(BLACK);
    DrawTextureEx(
        background_layer_1, (Vector2){0, settings.background_scrool_speed},
        0.0f, (float)settings.screen_width / background_layer_1.width, WHITE);
    DrawTextureEx(
        background_layer_2, (Vector2){0, settings.background_scrool_speed},
        0.0f, (float)settings.screen_width / background_layer_2.width, WHITE);
    DrawTextureEx(
        background_layer_3, (Vector2){0, settings.background_scrool_speed},
        0.0f, (float)settings.screen_width / background_layer_3.width, WHITE);
    DrawTextureEx(
        background_layer_4, (Vector2){0, settings.background_scrool_speed},
        0.0f, (float)settings.screen_width / background_layer_4.width, WHITE);
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
      DrawText("Game Mode", 20, 20, 40, RAYWHITE);
      if (IsKeyReleased(KEY_BACKSPACE)) {
        settings.mode = MAIN_MENU;
      };
    };
    // END OF ACTUAL GAME
    EndDrawing();
  }
  UnloadTexture(background_layer_1);
  UnloadTexture(background_layer_2);
  UnloadTexture(background_layer_3);
  UnloadTexture(background_layer_4);
  CloseWindow();
  return 0;
}
