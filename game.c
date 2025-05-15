#include "game.h"
#include "raylib.h"

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

int GameProcessShooting(PlayerSettings *player, int bullet_counter,
                        int max_bullets) {
  player->bullets[bullet_counter - 1].pos.y = player->position.y - 70.0f;
  player->bullets[bullet_counter - 1].pos.x = player->position.x;
  player->bullets[bullet_counter - 1].collider.x = player->position.x;
  player->bullets[bullet_counter - 1].collider.y = player->position.y - 70.0f;

  if (bullet_counter == 1) {
    return max_bullets;
  };

  return bullet_counter -= 1;
};

void GameCalculateBullets(PlayerSettings *player, float acc, int max_bullets,
                          int s_height) {
  for (int i = 0; i < max_bullets; i++) {
    if (player->bullets[i].pos.y <
        -s_height) { // set bullets to init points after reaching the out of
                     // bounds
      player->bullets[i].pos.y = player->position.y;
      player->bullets[i].pos.x = player->position.x;
      player->bullets[i].collider.x = player->position.x;
      player->bullets[i].collider.y = player->position.y;
      player->bullets[i].acceleration = 0.0f;
    };

    if (player->bullets[i].pos.y < player->position.y) {
      player->bullets[i].acceleration += acc * GetFrameTime();
    };

    player->bullets[i].pos.y -= player->bullets[i].acceleration;
    player->bullets[i].collider.y = player->bullets[i].pos.y;
  };
};
