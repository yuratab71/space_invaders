main: main.c
	gcc main.c -o 'SpaceInvaders.exe' -Iraylib/include -Lraylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
