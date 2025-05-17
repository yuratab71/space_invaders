#include "global_settings.h"
#include <stdbool.h>

void SettingsInit(GlobalSettings *settings) {
  settings->screen_width = 1024;
  settings->screen_height = 720;
  settings->mode = MENU;
  settings->is_paused = false;
  settings->should_close = false;
  return;
};
