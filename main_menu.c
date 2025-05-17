#include "main_menu.h"
#include "raylib.h"

void MenuInit(MenuSettings *menu, GlobalSettings *settings) {
  menu->title_fz = 30;
  menu->title_text = "Hello, Space Invaders";
  menu->btn_fsz = 20;
  menu->title_pos_x =
      CalculateXPos(menu->title_text, settings->screen_width, menu->btn_fsz);
  menu->title_pos_y = 400;
  menu->position_step_y = 8;
  menu->focus = 0;

  return;
};

void ButtonsInit(Btn btn[], GlobalSettings *settings, MenuSettings *menu) {
  int btn_size = 2;
  for (int i = 0; i < btn_size; i++) {
    btn[i].text = (i == 0 ? "Start" : "Exit");
    btn[i].step = menu->position_step_y + i + 1;
    btn[i].x_pos =
        CalculateXPos(btn[i].text, settings->screen_width, menu->btn_fsz);
    btn[i].y_pos =
        CalculateYPos(settings->screen_height, menu->position_step_y);
    btn[i].y_pos = 500;
    btn[i].is_focused = !i;
  };
};

int CalculateXPos(char text[], int s_width, int fsz) {
  return (int)(s_width - MeasureText(text, fsz)) / 2;
};

int CalculateYPos(int s_height, int step) { return (int)s_height / 16 * step; };

void MenuDrawTitle(MenuSettings *menu) {
  DrawText(menu->title_text, menu->title_pos_x, menu->title_pos_y,
           menu->title_fz, RAYWHITE);
  return;
};

void MenuDrawButtons(MenuSettings *menu, Btn btn[]) {
  DrawText(btn[0].text, btn[0].x_pos, btn[0].y_pos, menu->btn_fsz,
           menu->focus == 0 ? ORANGE : RAYWHITE);
  DrawText(btn[1].text, btn[1].x_pos, btn[1].y_pos + 30, menu->btn_fsz,
           menu->focus != 0 ? ORANGE : RAYWHITE);
  return;
};

void MenuProcessKey(int key, MenuSettings *menu, GlobalSettings *stn) {
  switch (key) {
  case KEY_DOWN:
    if (menu->focus == 1) {
      menu->focus = 0;
      break;
    };
    menu->focus += 1;
    break;
  case KEY_UP:
    if (menu->focus == 0) {
      menu->focus = 1;
      break;
    }
    menu->focus -= 1;
    break;
  case KEY_ENTER:
    if (menu->focus == 1) {
      stn->should_close = true;
      break;
    };
    if (menu->focus == 0) {
      stn->mode = GAME;
      break;
    };
    break;
  default:
    break;
  };
  return;
};
