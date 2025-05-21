#include "game.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>

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

void GameLoadEnemyTextures(EnemyTextures *textures) {
  textures->red = LoadTexture(
      "./assets/PixelSpaceRage/128px/Enemy01_Red_Frame_2_png_processed.png");
  textures->green = LoadTexture(
      "./assets/PixelSpaceRage/128px/Enemy01_Green_Frame_1_png_processed.png");
  textures->teal = LoadTexture(
      "./assets/PixelSpaceRage/128px/Enemy01_Teal_Frame_1_png_processed.png");
  textures->red_small = LoadTexture(
      "./assets/PixelSpaceRage/128px/Enemy02Red_Frame_1_png_processed.png");
  textures->projectile = LoadTexture(
      "./assets/PixelSpaceRage/128px/Plasma_Large_png_processed.png");
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

void GameUnloadEnemyTextures(EnemyTextures *textures) {
  UnloadTexture( textures->red);
  UnloadTexture(textures->green);
  UnloadTexture(textures->teal);
  UnloadTexture(textures->red_small);
  UnloadTexture(textures->projectile);
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
  player->score = 0;
  player->can_shoot = true;
  player->bullet.pos.x = player->position.x;
  player->bullet.pos.y = 700;
  player->bullet.collider.x = player->bullet.pos.x;
  player->bullet.collider.y = player->bullet.pos.y;
  player->bullet.collider.width = player->projectile.width;
  player->bullet.collider.height = player->projectile.height;
  player->collider.x = player->position.x - 15.0f;
  player->collider.y = player->position.y - 30.0f;
  player->collider.width = (float)player->idle.width / 2;
  player->collider.height = player->idle.height;
};

void GameInitEnemies(EnemiesSettings *settings, int enemy_x_length,
                     int enemy_y_len,
                     Enemy enemies[enemy_x_length][enemy_y_len], Enemy *wenemy,
                     Projectile *projectile, float start_pos_x) {
  settings->move_timer = 120.0f;
  settings->move_counter = 2;
  settings->move_dir = LEFT;
  settings->move_dirprev = LEFT;
  settings->move_step = 40.0f;
  settings->start_pos_x = start_pos_x;
  settings->is_wander = false;
  settings->is_wtimer = 0.0f;
  settings->can_shoot = false;
  settings->shoot_timer = 0.0f;

  for (int i = 0; i < enemy_x_length; i++) {
    for (int j = 0; j < enemy_y_len; j++) {
      enemies[i][j].collider.x = start_pos_x + j * 80 + 15.0f;
      enemies[i][j].collider.y = 50 + i * 50 + 10.0f;
      enemies[i][j].pos.x = start_pos_x + j * 80;
      enemies[i][j].pos.y = 50 + i * 50;

      enemies[i][j].collider.width = 35;
      enemies[i][j].collider.height = 40;

      enemies[i][j].bullet.x = 0;
      enemies[i][j].bullet.y = 0;
      enemies[i][j].bullet.height = 20;
      enemies[i][j].bullet.width = 20;

      enemies[i][j].is_alive = true;
    };
  };
  wenemy->pos.x = -50.0f;
  wenemy->pos.y = 40.0f;
  wenemy->collider.x = wenemy->pos.x;
  wenemy->collider.y = wenemy->pos.y;
  wenemy->collider.width = 30;
  wenemy->collider.height = 30;

  projectile->pos =
      GameGetRandomEnemyPosition(enemy_x_length, enemy_y_len, enemies);
  projectile->collider.x = projectile->pos.x;
  projectile->collider.y = projectile->pos.y;
  projectile->collider.width = 15;
  projectile->collider.height = 15;
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

void GameProcessEnemyGridMovement(
    EnemiesSettings *settings, int enemy_x_length, int enemy_y_length,
    Enemy enemies[enemy_x_length][enemy_y_length]) {
  if (settings->move_timer < 1.0f) {
    if (settings->move_counter == 0 &&
        (settings->move_dir == LEFT || settings->move_dir == RIGHT)) {
      settings->move_dirprev = settings->move_dir;
      settings->move_dir = DOWN;
    };
    switch (settings->move_dir) {
    case LEFT:
      for (int i = 0; i < enemy_x_length; i++) {
        for (int j = 0; j < enemy_y_length; j++) {
          enemies[i][j].collider.x -= settings->move_step;
          enemies[i][j].pos.x -= settings->move_step;
        };
      };
      settings->move_counter -= 1;
      settings->move_timer = 120.0f;
      return;
    case RIGHT:
      for (int i = 0; i < enemy_x_length; i++) {
        for (int j = 0; j < enemy_y_length; j++) {
          enemies[i][j].collider.x += settings->move_step;
          enemies[i][j].pos.x += settings->move_step;
        };
      };
      settings->move_counter -= 1;
      settings->move_timer = 120.0f;
      return;
    case DOWN:
      for (int i = 0; i < enemy_x_length; i++) {
        for (int j = 0; j < enemy_y_length; j++) {
          enemies[i][j].collider.y += settings->move_step;
          enemies[i][j].pos.y += settings->move_step;
        };
      };
      settings->move_counter = 5;
      settings->move_dir = settings->move_dirprev ? LEFT : RIGHT;
      settings->move_timer = 120.0f;
      return;
    default:
      return;
    };
  }
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

Vector2
GameGetRandomEnemyPosition(int enemy_x_length, int enemy_y_length,
                           Enemy enemies[enemy_x_length][enemy_y_length]) {
  int x = rand() % (enemy_x_length - 1 - 0 + 1) + 0;
  int y = rand() % (enemy_y_length - 1 - 0 + 1) + 0;

  Vector2 result = {enemies[x][y].pos.x, enemies[x][y].pos.y};
  return result;
};

void GameCalculatePlayer(PlayerSettings *player, float delta,
                         GlobalSettings *settings) {
  player->position.x += player->acceleration * delta;
  if (player->position.x < 0)
    player->position.x = settings->screen_width;
  if (player->position.x > settings->screen_width)
    player->position.x = 0;
  return;
};

void GameProcessCollisionBulletOnEnemy(PlayerSettings *player, Enemy *enemy) {
  if (enemy->is_alive &&
      CheckCollisionRecs(player->bullet.collider, enemy->collider)) {
    player->bullet.pos.x = player->position.x;
    player->bullet.pos.y = player->position.y;
    player->can_shoot = true;
    enemy->is_alive = false;
    player->score += 50;
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

void GameDrawEnemies(int enemy_x_axis, int enemy_y_axis,
                     Enemy enemies[enemy_x_axis][enemy_y_axis],
                     Texture2D *texture) {
  for (int i = 0; i < enemy_x_axis; i++) {
    for (int j = 0; j < enemy_y_axis; j++) {
      if (enemies[i][j].is_alive) {
        DrawRectangle(enemies[i][j].collider.x, enemies[i][j].collider.y,
                      enemies[i][j].collider.width,
                      enemies[i][j].collider.height, RAYWHITE);
        DrawTextureEx(*texture,
                      (Vector2){enemies[i][j].pos.x, enemies[i][j].pos.y}, 0.0f,
                      1.0f, WHITE);
      };
    };
  };
};
