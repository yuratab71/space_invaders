#include "global_settings.h"
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

void BackgroundLoadTexture(BackgroundSettings *background);
void BackgroundUnloadTexture(BackgroundSettings *background);
void BackgroundInit(BackgroundSettings *background, GlobalSettings *settings);
float BackgroundGetScale(int s_width, int b_width);
void BackgrounfCalculatePosition(BackgroundSettings *bstn);
void BackgroundDrawSelf(BackgroundSettings *bstn);

#endif
