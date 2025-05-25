#include <raylib.h>

#ifndef OBJECTS_H_
#define OBJECTS_H_

typedef struct {
  Rectangle collider; //texture would be 32x32
  int health; // from 4 to 1, 0 is death;
} DestroybleObject;

typedef struct {
  Texture2D full;
  Texture two_third;
  Texture2D one_third;
  Texture2D nead_death;
} ObjectTextures;

void ObjectsLoadTextures(ObjectTextures * texture);
void ObjectsUnloadTextures(ObjectTextures * texture);
void ObjectsInit(int x, int y, DestroybleObject[x][y]);
Texture2D GetTextureByHealth(int health, ObjectTextures *textures);

#endif
