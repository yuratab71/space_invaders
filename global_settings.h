#include <stdbool.h>

#ifndef GLOABAL_SETTINGS_H_
#define GLOABAL_SETTINGS_H_

#define SCREEN_WIDTH 1028;
#define SCREEN_HEIGHT 720;


enum Modes { MENU, GAME , GAME_OVER };

typedef struct {
  int screen_width;
  int screen_height;
  bool should_close;
  enum Modes mode;
  bool is_paused;
} GlobalSettings;

void SettingsInit(GlobalSettings *settings);

#endif
