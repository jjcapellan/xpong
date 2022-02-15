#include "defs.h"

// Resources
Texture2D texture_gameover;

void scene_gameover_init()
{
    texture_gameover = LoadTexture("assets/screen_gameover.png");
    ShowCursor();
}

void scene_gameover_update()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        scene_transition_init(SCENE_GAMEOVER, SCENE_GAMEPLAY);
    }
}

void scene_gameover_draw()
{
    DrawTexture(texture_gameover, 0, 0, WHITE);
}

void scene_gameover_destroy()
{
    UnloadTexture(texture_gameover);
}