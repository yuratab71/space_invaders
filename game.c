#include "game.h"
#include "raylib.h"
#include <stdbool.h>

void GameLoadPlayerTextures(PlayerSettings *player) {
  player->idle = LoadTexture(
      "./assets/PixelSpaceRage/128px/PlayerBlue_Frame_01_png_processed.png");
  player->right_1 =
      LoadTexture("./assets/PixelSpaceRage/128px/"
                  "PlayerBlue_Frame_02_png_processed_flipped.png");
  player->right_2 =
      LoadTexture("./assets/PixelSpaceRage/128px/"
                  "PlayerBlue_Frame_03_png_processed_flipped.png");
  player->left_1 = LoadTexture(
      "./assets/PixelSpaceRage/128px/PlayerBlue_Frame_02_png_processed.png");
  player->left_2 = LoadTexture(
      "./assets/PixelSpaceRage/128px/PlayerBlue_Frame_03_png_processed.png");
  player->projectile = LoadTexture(
      "./assets/PixelSpaceRage/128px/Proton_Large_png_processed.png");
  return;
};
void GameUnloadPlayerTextures(PlayerSettings *player) {
  UnloadTexture(player->idle);
  UnloadTexture(player->right_1);
  UnloadTexture(player->right_2);
  UnloadTexture(player->left_1);
  UnloadTexture(player->left_2);
  UnloadTexture(player->projectile);
};

void GameInitPlayer(PlayerSettings *player, GlobalSettings *settings) {
  player->position.x = (float)settings->screen_width / 2;
  player->position.y = settings->screen_height - 70;
  player->position.width = player->idle.width;
  player->position.height = player->idle.height;
  player->acceleration = 0.0f;
  player->acceleration_speed = 100.0f;
  player->decceleration_speed = 120.0f;
  player->max_acceleration = 270.0f;
  player->can_shoot = true;
  player->bullet.pos.x = player->position.x;
  player->bullet.pos.y = 700;
  player->bullet.collider.x = player->bullet.pos.x;
  player->bullet.collider.y = player->bullet.pos.y;
  player->bullet.collider.width = player->projectile.width;
  player->bullet.collider.height = player->projectile.height;
};

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

void GameCalculatePlayer(PlayerSettings *player, float delta) {
  player->position.x += player->acceleration * delta;
  return;
};

void GameProcessCollisionBulletOnEnemy(PlayerSettings *player, Enemy *enemy) {
  if (enemy->is_alive && CheckCollisionRecs(player->bullet.collider, enemy->collider)) {
    player->bullet.pos.x = player->position.x;
    player->bullet.pos.y = player->position.y;
    player->can_shoot = true;
    enemy->is_alive = false;
  };
};

void GameDrawPlayer(PlayerSettings *player, Rectangle source, Vector2 origin) {
  if (player->acceleration > 70.0f) {
    DrawTexturePro(player->right_2, source, player->position, origin, 0.0f,
                   WHITE);
    return;
  };
  if (player->acceleration > 30.0f) {
    DrawTexturePro(player->right_1, source, player->position, origin, 0.0f,
                   WHITE);
    return;
  };
  if (player->acceleration < -70.0f) {
    DrawTexturePro(player->left_2, source, player->position, origin, 0.0f,
                   WHITE);
    return;
  };
  if (player->acceleration < -30.0f) {
    DrawTexturePro(player->left_1, source, player->position, origin, 0.0f,
                   WHITE);
    return;
  };
  DrawTexturePro(player->idle, source, player->position, origin, 0.0f, WHITE);
  return;
};

void GameDrawPlayerBullet(PlayerSettings *player) {
  if (player->bullet.pos.y < player->position.y) {
    DrawTextureEx(player->projectile, player->bullet.pos, 0.0f, 1.0f, WHITE);
  };
  return;
};
