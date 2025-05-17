#include "raylib.h"
#include <stdbool.h>

#ifndef GAME_H_
#define GAME_H_

typedef struct {
  struct Vector2 pos;
  Rectangle collider;
} Projectile;

typedef struct {
  Rectangle collider;
  Rectangle bullet;
  bool is_alive;
} Enemy;

typedef struct {
  Rectangle position;
  float acceleration;
  float acceleration_speed;
  float decceleration_speed;
  float max_acceleration;
  Projectile bullet;
  int health;
  bool can_shoot;
} PlayerSettings;

typedef struct {} Enemie;

void GameProcessKeyMovement(int key, PlayerSettings *player);
void GameProcessShooting(PlayerSettings *player);
void GameCalculateBullets(PlayerSettings *player, float delta);

#endif
