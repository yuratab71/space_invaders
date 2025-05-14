#include "game.h"
#include "raylib.h"

void GameProcessKey(int key, PlayerSettings *player) {
  switch (key) {
  case KEY_LEFT:
    if (player->acceleration <= 0.0f - player->max_acceleration)
      break;

    player->acceleration -= player->acceleration_speed * GetFrameTime();
    break;
  case KEY_RIGHT:
    if (player->acceleration >= player->max_acceleration)
      break;

    player->acceleration += player->acceleration_speed * GetFrameTime();
    break;
  default:
    break;
  };
  return;
};
