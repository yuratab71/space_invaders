#include "raylib.h"

#ifndef AUDIO_PLAYER_H_
#define AUDIO_PLAYER_H_

typedef struct {
  Music main_theme;
  Sound player_gun;
  Sound enemy_gun;
  Sound explosion_1;
  Sound explosion_2;
  Sound explosion_3;
} AudioPlayer;

void AudioPlayerLoadAssets(AudioPlayer *audio);
void AudioPlayerUnloadAssets(AudioPlayer *audio);
void AudioPlayerPlayExplosion(AudioPlayer *audio);

#endif
