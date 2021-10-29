#include "defs.h"

// Resources
Texture2D texture_gameover;

void scene_gameover_init()
{
    texture_gameover = LoadTexture("assets/screen_gameover.png");
}

void scene_gameover_update()
{
    if (IsKeyDown(KEY_SPACE))
    {
        current_scene = SCENE_TRANSITION;
        scene_transition_init(SCENE_GAMEOVER, SCENE_GAMEPLAY);
    }
}

void scene_gameover_draw()
{
    //BeginDrawing();
    //ClearBackground(BLACK);
    DrawTexture(texture_gameover, 0, 0, WHITE);
    //EndDrawing();
}

void scene_gameover_destroy()
{
    UnloadTexture(texture_gameover);
}