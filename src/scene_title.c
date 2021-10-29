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
        current_scene = SCENE_TRANSITION;
        scene_transition_init(SCENE_TITLE, SCENE_GAMEPLAY);
    }
}

void scene_title_draw()
{
    DrawTexture(texture_title, 0, 0, WHITE);
}

void scene_title_destroy()
{
    UnloadTexture(texture_title);
}
