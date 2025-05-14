#include "raylib.h"

#ifndef GAME_H_
#define GAME_H_

typedef struct {
  struct Vector2 pos;
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

void GameProcessKey(int key, PlayerSettings *player);

#endif
