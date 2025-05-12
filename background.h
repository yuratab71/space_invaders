#include "raylib.h"
#include <stdbool.h>

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

typedef struct {
  float scroll_speed;
  float y_pos;
  float scale;
  Texture2D texture;
} BackgroundSettings;

float get_background_scale(int swidth, int bwidth);

void CalculateBackgroundPosition(BackgroundSettings* bstn);

void DrawBackground(BackgroundSettings* bstn);

#endif
