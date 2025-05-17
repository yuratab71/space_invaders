#include <stdbool.h>

#ifndef GLOABAL_SETTINGS_H_
#define GLOABAL_SETTINGS_H_

enum Modes { MENU, GAME };

typedef struct {
  int screen_width;
  int screen_height;
  bool should_close;
  enum Modes mode;
  bool is_paused;
} GlobalSettings;

void SettingsInit(GlobalSettings *settings);

#endif
