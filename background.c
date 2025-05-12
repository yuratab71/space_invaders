#include "background.h"
#include "raylib.h"
#include <stdbool.h>

float GetBackgroundScale(int s_width, int b_width) {
  return (float) s_width / b_width;
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
