#include "background.h"
#include "global_settings.h"
#include "raylib.h"
#include <stdbool.h>

void
BackgroundLoadTexture (BackgroundSettings *background)
{
  background->texture = LoadTexture (
      "./assets/PixelSpaceRage/PixelBackgroundSeamlessVertically.png");
  return;
};

void
BackgroundUnloadTexture (BackgroundSettings *background)
{
  UnloadTexture (background->texture);
  return;
};

void
BackgroundInit (BackgroundSettings *background, GlobalSettings *settings)
{
  background->y_pos = 0.0f;
  background->scroll_speed = 0.5f;
  background->scale
      = BackgroundGetScale (settings->screen_width, background->texture.width);
  return;
};

float
BackgroundGetScale (int s_width, int b_width)
{
  return (float)s_width / b_width;
};

void
BackgrounfCalculatePosition (BackgroundSettings *bstn)
{
  if (bstn->y_pos >= bstn->texture.height * bstn->scale)
    {
      bstn->y_pos = 0.0f;
    }

  bstn->y_pos += bstn->scroll_speed;
  return;
};

void
BackgroundDrawSelf (BackgroundSettings *bstn)
{
  DrawTextureEx (bstn->texture, (Vector2){ 0, bstn->y_pos }, 0.0f, bstn->scale,
                 WHITE);
  DrawTextureEx (
      bstn->texture,
      (Vector2){ 0, bstn->y_pos - bstn->texture.height * bstn->scale }, 0.0f,
      bstn->scale, WHITE);

  return;
};
