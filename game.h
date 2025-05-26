#include <stdbool.h>

#include "global_settings.h"
#include "audio_player.h"
#include "raylib.h"

#ifndef GAME_H_
#define GAME_H_

#define ENEMY_ON_X 7;  // TODO need to switch it in code, now the x is y in for enemies
#define ENEMY_ON_Y 8;
#define ENEMY_MOVE_TIME 60.0f;
#define WENEMY_TIME 100.0f;
#define WENEMY_TIME_MAX 1200.0f;
#define WENEMY_MOVE 100.0f;
#define ENEMY_STEP 15.0f;

#define PLAYER_ACCELERATION_SPEED 100.0f;
#define PLAYER_DECELERATION_SPEED 120.0f;
#define PLAYER_MAX_ACCELERATION 270.0f;
#define PLAYER_BULLET_SPEED 750.0f;

enum EnemyMovement { LEFT, RIGHT, DOWN };

typedef struct {
  struct Vector2 pos;
  Rectangle collider;
} Projectile;

typedef struct {
  float timer;
  Vector2 pos;
} AnimationManager;

typedef struct {
  Rectangle collider;
  Vector2 pos;
  Rectangle bullet;
  bool is_alive;
} Enemy;

typedef struct {
  Texture2D red;
  Texture2D green;
  Texture2D teal;
  Texture2D red_small;
  Texture2D projectile;
} EnemyTextures;

typedef struct {
  Rectangle position;
  float acceleration;
  float acceleration_speed;
  float decceleration_speed;
  float max_acceleration;
  unsigned int score;
  Projectile bullet;
  Rectangle collider;
  bool can_shoot;
  Texture2D idle;
  Texture2D right_1;
  Texture2D right_2;
  Texture2D left_1;
  Texture2D left_2;
  Texture2D projectile;
  Vector2 origin;
  Rectangle source_rec;
} PlayerSettings;

typedef struct {
  float move_timer;
  int move_counter;
  int move_dirprev;
  int move_dir;
  float move_step;
  int start_pos_x;

  bool is_wander;
  float is_wtimer;

  bool can_shoot;
  float shoot_timer;
} EnemiesSettings;

typedef struct {
  Texture2D exp_1;
  Texture2D exp_2;
  Texture2D exp_3;
  Texture2D exp_4;
  Texture2D exp_5;
  Texture2D exp_6;
  Texture2D exp_7;
  Texture2D exp_8;
} ExplosionTextures;

void GameLoadPlayerTextures(PlayerSettings *player);
void GameLoadEnemyTextures(EnemyTextures *textures);
void GameLoadExplosionTextures(ExplosionTextures *textures);
void GameUnloadEnemyTextures(EnemyTextures *textures);
void GameUnloadPlayerTextures(PlayerSettings *player);
void GameUnloadExplosionTextures(ExplosionTextures *textures);
void GameInitPlayer(PlayerSettings *player, GlobalSettings *settings);

void GameInitEnemies(EnemiesSettings *settings, int enemy_x_length,
                     int enemy_y_len,
                     Enemy enemies[enemy_x_length][enemy_y_len], Enemy *wenemy,
                     Projectile *projectile, float start_pos_x);
void GameProcessKeyMovement(int key, PlayerSettings *player);
void GameProcessEnemyGridMovement(EnemiesSettings *settings, int enemy_x_length,
                                  int enemy_y_len,
                                  Enemy enemies[enemy_x_length][enemy_y_len]);
void GameProcessShooting(PlayerSettings *player);
void GameCalculateBullets(PlayerSettings *player, float delta);
Vector2 GameGetRandomEnemyPosition(int enemy_x_length, int enemy_y_len,
                                   Enemy enemies[enemy_x_length][enemy_y_len]);
void GameCalculatePlayer(PlayerSettings *player, float delta,
                         GlobalSettings *settings);
bool GameProcessCollisionBulletOnEnemy(PlayerSettings *player, Enemy *enemy, AudioPlayer *audio);
void GameDrawPlayer(PlayerSettings *player);
void GameDrawPlayerBullet(PlayerSettings *player);
void GameDrawEnemies(int enemy_x_length, int enemy_y_length,
                     Enemy enemies[enemy_x_length][enemy_y_length],
                     Texture2D *texture);
void GameDrawWEnemy(Enemy *wenemy, Texture2D *texture);
#endif
