#include "raylib.h"

#ifndef GAME_H_
#define GAME_H_

typedef struct {
  struct Vector2 pos;
  Rectangle collider;
  float acceleration;
} Projectile;

typedef struct {
  Rectangle position;
  float acceleration;
  float acceleration_speed;
  float decceleration_speed;
  float max_acceleration;
  Projectile bullets[8];
  int health;
} PlayerSettings;

void GameProcessKeyMovement(int key, PlayerSettings *player);
int GameProcessShooting(PlayerSettings *player, int bullet_counter, int max_bullets);
void GameCalculateBullets(PlayerSettings *player, float acc, int max_bullets, int s_height); 

#endif
