#include "defs.h"

// Resources
Texture2D texture_title;

void scene_title_init()
{
    texture_title = LoadTexture("assets/screen_title.png");
}

void scene_title_update()
{
    if (IsKeyDown(KEY_SPACE))
    {
        scene_gameplay_init();
        scene_title_destroy();
        current_scene = SCENE_GAMEPLAY;
    }
}

void scene_title_draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(texture_title, 0, 0, WHITE);
    EndDrawing();
}

void scene_title_destroy()
{
    UnloadTexture(texture_title);
}
