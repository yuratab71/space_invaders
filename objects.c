#include "objects.h"
#include "raylib.h"

Vector2 get_obj_coordinates(int pos, int map);

void ObjectsLoadTextures(ObjectTextures *texture)
{
    texture->full = LoadTexture("./assets/Blocks/Block_full.png");
    texture->two_third = LoadTexture("./assets/Blocks/Block_2_third_health.png");
    texture->one_third = LoadTexture("./assets/Blocks/Block_1_third_health.png");
    texture->nead_death = LoadTexture("./assets/Blocks/Block_near_death_health.png");
    return;
};

void ObjectsUnloadTextures(ObjectTextures *texture)
{
    UnloadTexture(texture->full);
    UnloadTexture(texture->two_third);
    UnloadTexture(texture->one_third);
    UnloadTexture(texture->nead_death);
    return;
};

void ObjectsInit(int x, int y, DestroybleObject obj[x][y])
{
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        { // for x = 1
            obj[i][j].health = 4;
            obj[i][j].collider.x = (7 * (i + 1) + (j + 1)) * 32;
            obj[i][j].collider.y = 510;
            obj[i][j].collider.height = 32;
            obj[i][j].collider.width = 32;
        };
    };
};

Texture2D GetTextureByHealth(int health, ObjectTextures *textures)
{
    switch (health)
    {
    case 1:
        return textures->nead_death;
    case 2:
        return textures->one_third;
    case 3:
        return textures->two_third;
    case 4:
        return textures->full;
    default:
        return textures->full;
    };
};
