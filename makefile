main: main.c
	gcc main.c global_settings.c background.c main_menu.c game.c audio_player.c -Wall -Wextra -std=c99 -o  'SpaceInvaders.exe' -Iraylib/include -Lraylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
