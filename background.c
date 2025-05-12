#include "background.h"
#include "raylib.h"
#include <stdbool.h>

float get_background_scale(int swidth, int bwidth) {
  return (float)swidth / bwidth;
};

void CalculateBackgroundPosition(BackgroundSettings *bstn) {
  if (bstn->y_pos >= bstn->texture.height * bstn->scale) {
    bstn->y_pos = 0.0f;
  }

  bstn->y_pos += bstn->scroll_speed;
  return;
};

void DrawBackground(BackgroundSettings *bstn) {
  DrawTextureEx(bstn->texture, (Vector2){0, bstn->y_pos}, 0.0f, bstn->scale,
                WHITE);
  DrawTextureEx(bstn->texture,
                (Vector2){0, bstn->y_pos - bstn->texture.height * bstn->scale},
                0.0f, bstn->scale, WHITE);

  return;
};
