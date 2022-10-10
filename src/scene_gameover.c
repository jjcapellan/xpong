#include "defs.h"
#include "gameover.h"


int score_width;
int final_points;

// Resources
Texture2D texture_gameover;

void scene_gameover_init()
{
    texture_gameover = LoadTexture("assets/screen_gameover.png");
    score_width = MeasureText("0000 POINTS", 36);
    final_points = player_total_score * 100 - npc_total_score *50 + player_max_level * 1000;
    ShowCursor();
}

void scene_gameover_update()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        scene_transition_init(SCENE_GAMEOVER, SCENE_GAMEPLAY);
    }
}

void scene_gameover_draw()
{
    DrawTexture(texture_gameover, 0, 0, WHITE);
    DrawTextEx(font, TextFormat("%i points", final_points), (Vector2){320 - (float)score_width / 2, 480 - 120}, 36, -3, (Color){124, 63, 88, 255});
}

void scene_gameover_destroy()
{
    UnloadTexture(texture_gameover);
}