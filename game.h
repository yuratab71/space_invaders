#include "global_settings.h"
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
  Vector2 pos;
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
  Texture2D idle;
  Texture2D right_1;
  Texture2D right_2;
  Texture2D left_1;
  Texture2D left_2;
  Texture2D projectile;
} PlayerSettings;

void GameLoadPlayerTextures(PlayerSettings *player);
void GameUnloadPlayerTextures(PlayerSettings *player);
void GameInitPlayer(PlayerSettings *player, GlobalSettings *settings);
void GameProcessKeyMovement(int key, PlayerSettings *player);
void GameProcessShooting(PlayerSettings *player);
void GameCalculateBullets(PlayerSettings *player, float delta);
Vector2 GameGetRandomEnemyPosition(Enemy enemies[6][5]);
void GameCalculatePlayer(PlayerSettings *player, float delta, GlobalSettings *settings);
void GameProcessCollisionBulletOnEnemy(PlayerSettings *player, Enemy *enemy);
void GameDrawPlayer(PlayerSettings *player, Rectangle source, Vector2 origin);
void GameDrawPlayerBullet(PlayerSettings *player);
void GameDrawEnemies(Enemy enemies[6][5], Texture2D *texture);
#endif
