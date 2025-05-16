#include "game.h"
#include "raylib.h"
#include <stdbool.h>

void GameProcessKeyMovement(int key, PlayerSettings *player) {
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

void GameProcessShooting(PlayerSettings *player) {
  player->bullet.pos.y = player->position.y - 70.0f;
  player->bullet.pos.x = player->position.x;
  player->bullet.collider.x = player->position.x;
  player->bullet.collider.y = player->position.y - 70.0f;
  player->can_shoot = false;
  return;
};

void GameCalculateBullets(PlayerSettings *player, float delta) {
  if (player->bullet.pos.y < 0.0f) { // set bullets to init points after
                                          // reaching the out of bounds
    player->bullet.pos.y = player->position.y;
    player->bullet.pos.x = player->position.x;
    player->bullet.collider.x = player->position.x;
    player->bullet.collider.y = player->position.y;
    player->can_shoot = true;
  };
  if (player->bullet.pos.y < player->position.y) {
    player->bullet.pos.y -= 450.0f * delta;
  };

  player->bullet.collider.y = player->bullet.pos.y;
  return;
};
