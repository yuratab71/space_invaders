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

float GetBackgroundScale(int s_width, int b_width);

void CalculateBackgroundPosition(BackgroundSettings* bstn);

void DrawBackground(BackgroundSettings* bstn);

#endif
