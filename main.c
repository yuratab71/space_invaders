#include <stdbool.h>
#include <stdio.h>

#include "audio_player.h"
#include "background.h"
#include "game.h"
#include "global_settings.h"
#include "main_menu.h"
#include "objects.h"
#include "raylib.h"

#ifdef __cplusplus
extern "C"
{
#endif

  __declspec (dllexport) unsigned long NvOptimusEnablement = 1;
  __declspec (dllexport) int AmdPowerXpressRequestHighPerformance = 1;

#ifdef __cplusplus

#endif

  int enemy_on_x;
  int enemy_on_y;

  int objects_on_map;
  int objects_in_block;

  PlayerSettings player;
  EnemiesSettings e_settings;
  EnemyTextures e_textures;
  ObjectTextures obj_textures;
  GlobalSettings settings;
  BackgroundSettings background;
  AudioPlayer audio;
  ExplosionTextures exp_textures;
  MenuSettings menu;
  Btn buttons[2];
  Projectile enemy_projectile;
  Enemy wenemy;
  char title[] = "Space Invaders";
  char main_menu_title[] = "Hello, Space Invaders";

  int
  main (void)
  {
    if (NvOptimusEnablement && AmdPowerXpressRequestHighPerformance)
      {
        printf ("[GAME]: Use dedicated GPU enabled \n");
      };
    enemy_on_x = ENEMY_ON_X;
    enemy_on_y = ENEMY_ON_Y;
    Enemy enemies[enemy_on_x][enemy_on_y];
    int animation_counter = 0;

    AnimationManager animations[4] = { {
                                           .timer = 0.0f,
                                       },
                                       {
                                           .timer = 0.0f,
                                       },
                                       {

                                           .timer = 0.0f,
                                       },
                                       {

                                           .timer = 0.0f,
                                       }

    };
    objects_on_map = 3;
    objects_in_block = 3;
    DestroybleObject des_objects[objects_on_map][objects_in_block];
    SettingsInit (&settings);
    InitWindow (settings.screen_width, settings.screen_height, title);
    InitAudioDevice ();
    AudioPlayerLoadAssets (&audio);
    PlayMusicStream (audio.main_theme);
    float mt_time_played = 0.0f;
    SetWindowState (FLAG_VSYNC_HINT);
    BackgroundLoadTexture (&background);
    BackgroundInit (&background, &settings);
    MenuInit (&menu, &settings);
    MenuInitButtons (buttons, &settings, &menu);
    GameLoadPlayerTextures (&player);
    GameInitPlayer (&player, &settings);

    GameLoadExplosionTextures (&exp_textures);

    // Enemy init
    GameInitEnemies (&e_settings, enemy_on_x, enemy_on_y, enemies, &wenemy,
                     &enemy_projectile,
                     (float)settings.screen_width / 2
                         - ((float)enemy_on_x / 2) * 80);
    GameLoadEnemyTextures (&e_textures);
    ObjectsLoadTextures (&obj_textures);
    ObjectsInit (objects_on_map, objects_in_block, des_objects);

    while (!settings.should_close && !WindowShouldClose ())
      {
        float delta = GetFrameTime ();

        UpdateMusicStream (audio.main_theme);

        if (!settings.is_paused)
          {
            BackgrounfCalculatePosition (&background);
          };
        mt_time_played = GetMusicTimePlayed (audio.main_theme)
                         / GetMusicTimeLength (audio.main_theme);
        if (mt_time_played > 1.0f)
          mt_time_played = 0.0f;
        BeginDrawing ();

        ClearBackground (BLACK);
        BackgroundDrawSelf (&background);

        // MAIN MENU
        if (settings.mode == MENU)
          {
            MenuDrawTitle (&menu);
            MenuDrawButtons (&menu, buttons);
            if (IsKeyPressed (KEY_DOWN))
              MenuProcessKey (KEY_DOWN, &menu, &settings);
            if (IsKeyPressed (KEY_UP))
              MenuProcessKey (KEY_UP, &menu, &settings);
            if (IsKeyPressed (KEY_ENTER))
              MenuProcessKey (KEY_ENTER, &menu, &settings);
          };
        // END OF MAIN MENU

        // ACTUAL GAME
        if (settings.mode == GAME)
          {
            if (settings.is_paused)
              DrawText ("|| PAUSE", settings.screen_width - 180, 50, 20,
                        RAYWHITE);
            if (!settings.is_paused)
              {
                GameCalculateBullets (&player, delta);
                GameCalculatePlayer (&player, delta, &settings);

                e_settings.move_timer -= delta * ENEMY_MOVE_TIME;

                if (!e_settings.is_wander)
                  {
                    e_settings.is_wtimer += delta * WENEMY_TIME;
                  }
                else
                  {
                    if (wenemy.pos.x >= settings.screen_width)
                      {
                        e_settings.is_wander = false;
                      };
                    wenemy.pos.x += delta * WENEMY_MOVE;
                    wenemy.collider.x = wenemy.pos.x + 15.0f;
                  };
                if (!e_settings.is_wander && (e_settings.is_wtimer > 1200.0f))
                  {
                    wenemy.pos.x = -50.0f;
                    e_settings.is_wander = true;
                    e_settings.is_wtimer = 0.0f;
                  };

                if (e_settings.can_shoot)
                  {
                    e_settings.shoot_timer += 400.0f * delta;
                  };
                if (e_settings.can_shoot && e_settings.shoot_timer >= 500.0f)
                  {
                    enemy_projectile.pos = GameGetRandomEnemyPosition (
                        enemy_on_x, enemy_on_y, enemies);
                    enemy_projectile.collider.x
                        = enemy_projectile.pos.x - 12.0f;
                    enemy_projectile.collider.y
                        = enemy_projectile.pos.y - 12.0f;
                    e_settings.can_shoot = false;
                    e_settings.shoot_timer = 0.0f;
                  }
                else
                  {
                    if (enemy_projectile.pos.y > settings.screen_height)
                      e_settings.can_shoot = true;
                    enemy_projectile.pos.y += 300.0f * delta;
                    enemy_projectile.collider.y
                        = enemy_projectile.pos.y - 15.0f;
                  };
                if (CheckCollisionRecs (player.collider,
                                        enemy_projectile.collider))
                  {
                    settings.is_paused = true;
                    GameInitPlayer (&player, &settings);
                    GameInitEnemies (&e_settings, enemy_on_x, enemy_on_y,
                                     enemies, &wenemy, &enemy_projectile,
                                     (float)settings.screen_width / 2
                                         - ((float)enemy_on_x / 2) * 80);
                    ObjectsInit (objects_on_map, objects_in_block,
                                 des_objects);
                  };
                for (int i = 0; i < 4; i++)
                  {

                    if (animations[i].timer < 1.0f)
                      {
                        animations[i].timer = 0.0f;
                      }
                    else
                      {
                        animations[i].timer -= 1000.0f * delta;
                      };
                  };
                DrawText (TextFormat ("Counter = %d", animation_counter), 720,
                          160, 20, RAYWHITE);
                for (int i = 0; i < objects_on_map; i++)
                  {
                    for (int j = 0; j < objects_in_block; j++)
                      {
                        if (des_objects[i][j].health != 0)
                          if (!player.can_shoot)
                            {
                              if (CheckCollisionRecs (
                                      player.bullet.collider,
                                      des_objects[i][j].collider))
                                {
                                  des_objects[i][j].health -= 1;
                                  player.bullet.pos.x = player.position.x;
                                  player.bullet.pos.y = player.position.y;
                                  player.can_shoot = true;
                                };
                            };
                        if (!e_settings.can_shoot && des_objects[i][j].health)
                          {
                            {
                              if (CheckCollisionRecs (
                                      des_objects[i][j].collider,
                                      enemy_projectile.collider))
                                {
                                  des_objects[i][j].health -= 1;
                                  e_settings.can_shoot = true;
                                };
                            };
                          };
                      };
                  };
                if (CheckCollisionRecs (player.bullet.collider,
                                        wenemy.collider))
                  {
                    e_settings.is_wander = false;
                    e_settings.is_wtimer = 0.0f;
                    player.score += 300;
                    AudioPlayerPlayExplosion (&audio);
                  };

                for (int i = 0; i < enemy_on_x; i++)
                  {
                    for (int j = 0; j < enemy_on_y; j++)
                      {
                        if (GameProcessCollisionBulletOnEnemy (
                                &player, &enemies[i][j], &audio))
                          {
                            printf ("Hit\n");
                            animations[animation_counter].timer = 1200.0f;
                            animations[animation_counter].pos
                                = enemies[i][j].pos;
                            if (animation_counter == 3)
                              {
                                animation_counter = 0;
                              }
                            else
                              {
                                animation_counter += 1;
                              }
                            printf ("Timer is %f\n",
                                    animations[animation_counter].timer);
                          };
                      };
                  };
                GameProcessEnemyGridMovement (&e_settings, enemy_on_x,
                                              enemy_on_y, enemies);
              };
            // DRAWING SECTION
            //
            for (int i = 0; i < 4; i++)
              {
                if (animations[i].timer > 1150.0f)
                  {
                    DrawTexture (exp_textures.exp_1, animations[i].pos.x,
                                 animations[i].pos.y, WHITE);
                  }
                else if (animations[i].timer > 1000.0f)
                  {
                    DrawTexture (exp_textures.exp_2, animations[i].pos.x,
                                 animations[i].pos.y, WHITE);
                  }
                else if (animations[i].timer > 850.0f)
                  {
                    DrawTexture (exp_textures.exp_3, animations[i].pos.x,
                                 animations[i].pos.y, WHITE);
                  }
                else if (animations[i].timer > 600.0f)
                  {
                    DrawTexture (exp_textures.exp_4, animations[i].pos.x,
                                 animations[i].pos.y, WHITE);
                  }
                else if (animations[i].timer > 450.0f)
                  {
                    DrawTexture (exp_textures.exp_5, animations[i].pos.x,
                                 animations[i].pos.y, WHITE);
                  }
                else if (animations[i].timer > 300.0f)
                  {
                    DrawTexture (exp_textures.exp_1, animations[i].pos.x,
                                 animations[i].pos.y, WHITE);
                  }
                else if (animations[i].timer > 1150.0f)
                  {
                    DrawTexture (exp_textures.exp_1, animations[i].pos.x,
                                 animations[i].pos.y, WHITE);
                  }
                else if (animations[i].timer > 1150.0f)
                  {
                    DrawTexture (exp_textures.exp_1, animations[i].pos.x,
                                 animations[i].pos.y, WHITE);
                  }
                DrawText (TextFormat ("Animation timer no. %d = %f", i + 1,
                                      animations[i].timer),
                          720, 50 + i * 30, 20, RAYWHITE);
              };
            DrawText (TextFormat ("SCORE %d", player.score), 50, 50, 20,
                      RAYWHITE);
            GameDrawPlayer (&player);
            GameDrawPlayerBullet (&player);

            if (!e_settings.can_shoot)
              {
                DrawTextureEx (e_textures.projectile, enemy_projectile.pos,
                               180.0f, 1.0f, WHITE);
              };
            if (e_settings.is_wander)
              {
                GameDrawWEnemy (&wenemy, &e_textures.red_small);
              };
            GameDrawEnemies (enemy_on_x, enemy_on_y, enemies, &e_textures.red);
            for (int i = 0; i < 3; i++)
              {
                for (int j = 0; j < 3; j++)
                  {
                    if (des_objects[i][j].health != 0)
                      {
                        DrawTexture (
                            GetTextureByHealth (des_objects[i][j].health,
                                                &obj_textures),
                            des_objects[i][j].collider.x,
                            des_objects[i][j].collider.y, WHITE);
                      };
                  };
              };
            // INPUT SECTION
            if (IsKeyReleased (KEY_BACKSPACE))
              {
                settings.mode = MENU;
                GameInitPlayer (&player, &settings);
                GameInitEnemies (&e_settings, enemy_on_x, enemy_on_y, enemies,
                                 &wenemy, &enemy_projectile,
                                 (float)settings.screen_width / 2
                                     - ((float)enemy_on_x / 2) * 80);
                ObjectsInit (objects_on_map, objects_in_block, des_objects);
              };

            if (IsKeyPressed (KEY_SPACE) && !settings.is_paused
                && player.can_shoot)
              {
                GameProcessShooting (&player);
              };

            if (IsKeyDown (KEY_LEFT) && !settings.is_paused)
              {
                GameProcessKeyMovement (KEY_LEFT, &player);
              }
            else
              {
                if (player.acceleration < 0.0f && !settings.is_paused)
                  {
                    if (player.acceleration > -2.5f)
                      {
                        player.acceleration = 0.0f;
                      }
                    else
                      {
                        player.acceleration
                            += player.decceleration_speed * delta;
                      };
                  };
              };
            if (IsKeyDown (KEY_RIGHT) && !settings.is_paused)
              {
                GameProcessKeyMovement (KEY_RIGHT, &player);
              }
            else
              {
                if (player.acceleration > 0.0f && !settings.is_paused)
                  {
                    if (player.acceleration < 2.5f)
                      {
                        player.acceleration = 0.0f;
                      }
                    else
                      {
                        player.acceleration
                            -= player.decceleration_speed * delta;
                      };
                  };
              };
            if (IsKeyPressed (KEY_P))
              settings.is_paused = !settings.is_paused;
          };
        // INPUT END
        //
        // END OF ACTUAL GAME
        EndDrawing ();
      };
    BackgroundUnloadTexture (&background);
    GameUnloadPlayerTextures (&player);
    GameUnloadEnemyTextures (&e_textures);
    ObjectsUnloadTextures (&obj_textures);
    GameUnloadExplosionTextures (&exp_textures);
    AudioPlayerUnloadAssets (&audio);
    CloseAudioDevice ();
    CloseWindow ();
    return 0;
  };
