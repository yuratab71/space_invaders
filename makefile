main: main.c
	gcc main.c background.c main_menu.c -Wall -Wextra -std=c99 -o  'SpaceInvaders.exe' -Iraylib/include -Lraylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
