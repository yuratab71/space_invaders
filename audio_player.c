#include "audio_player.h"
#include <raylib.h>

void
AudioPlayerLoadAssets (AudioPlayer *audio)
{
  audio->main_theme
      = LoadMusicStream ("./music/10-theme-main-101soundboards.mp3");
  audio->explosion_1
      = LoadSound ("./music/spaceship/mp3/explosion/explosion1.mp3");
  audio->explosion_2
      = LoadSound ("./music/spaceship/mp3/explosion/explosion8.mp3");
  audio->explosion_3
      = LoadSound ("./music/spaceship/mp3/explosion/explosion11.mp3");
  return;
  audio->player_gun
      = LoadSound ("./music/spaceship/mp3/laser/laser_shoot7.mp3");
  audio->enemy_gun = LoadSound ("./music/spaceship/mp3/laser/shoot.mp3");
};

void
AudioPlayerUnloadAssets (AudioPlayer *audio)
{
  UnloadMusicStream (audio->main_theme);
  UnloadSound (audio->explosion_1);
  UnloadSound (audio->explosion_2);
  UnloadSound (audio->explosion_3);
  UnloadSound (audio->player_gun);
  UnloadSound (audio->enemy_gun);
};

void
AudioPlayerPlayExplosion (AudioPlayer *audio)
{
  static int exp_count = 1;
  switch (exp_count)
    {
    case 1:
      PlaySound (audio->explosion_1);
      exp_count += 1;
      return;
    case 2:
      PlaySound (audio->explosion_2);
      exp_count += 1;
      return;
    case 3:
      PlaySound (audio->explosion_3);
      exp_count = 1;
      return;
    default:
      return;
    };
  return;
};
