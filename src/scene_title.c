#include "defs.h"

// Resources
Texture2D texture_atlas; // Global

// Local variables
const char *LABEL;
int label_width;

void scene_title_init()
{
    LABEL = "Press space to play";
    label_width = MeasureText(LABEL, 32);
}

void scene_title_update()
{
    if (IsKeyDown(KEY_SPACE))
    {
        scene_gameplay_init();
        current_scene = SCENE_GAMEPLAY;
    }
}

void scene_title_draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTextureRec(texture_atlas, (Rectangle){0, 0, 640, 480}, (Vector2){0, 0}, WHITE);
    DrawText(LABEL, SCREEN_WIDTH / 2 - label_width / 2, 110, 32, CLITERAL(Color){124,63,88,255});
    EndDrawing();
}

void scene_title_destroy()
{
    // TO DO
}
