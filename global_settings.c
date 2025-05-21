#include "global_settings.h"
#include <stdbool.h>

void SettingsInit(GlobalSettings *settings) {
  settings->screen_width = SCREEN_WIDTH;
  settings->screen_height = SCREEN_HEIGHT;
  settings->mode = MENU;
  settings->is_paused = false;
  settings->should_close = false;
  return;
};
