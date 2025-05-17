#include "global_settings.h"
#include <stdbool.h>

#ifndef MAIN_MENU_H_
#define MAIN_MENU_H_

typedef struct {
  char text[16];
  int x_pos;
  int y_pos;
  bool is_focused;
} MainMenuBtn;

typedef struct {
  int y_pos;
  int x_pos;
  char *text;
  bool is_focused;
  int step;
} Btn;

typedef struct {
  int title_pos_y;
  int title_pos_x;
  int title_fz;
  char *title_text;

  int btn_fsz;
  int position_step_y;
  int focus;
} MenuSettings;

void MenuInit(MenuSettings *menu, GlobalSettings *settings);
void MenuInitButtons(Btn btn[], GlobalSettings *settings, MenuSettings *menu);

int MenuCalculateXPos(char *text, int s_width, int fsz);
int MenuCalculateYPos(int s_height, int step);

void MenuDrawTitle(MenuSettings *menu);
void MenuDrawButtons(MenuSettings *menu, Btn btn[]);

void MenuProcessKey(int key, MenuSettings *menu, GlobalSettings *stn);
#endif
