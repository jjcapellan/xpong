#include "defs.h"
#include "gameplay.h"


//
// GLOBALS
//

Entity player;
Entity npc;
Entity ball;


//
// LOCAL VARIABLES
//

Vector2 score_size;
Vector2 level_text_size;
int npc_score;
float paddle_speed;
int ball_speed;
int player_score;
int score_text_y = (SCREEN_HEIGHT / 2) - (SCORE_TEXT_SIZE / 2);
int level;


//
// LOCAL FUNCTIONS
//

void input_update();
void check_gameover();
void new_level();

//
// RESOURCES
//

Sound fx_bounce1;
Sound fx_bounce2;
Sound fx_point;
Sound fx_level;
Font font;
Texture2D texture_atlas; // Global


//
// FUNCTIONS
//
void scene_gameplay_init()
{
    fx_bounce1 = LoadSound("assets/bounce1.wav");
    fx_bounce2 = LoadSound("assets/bounce2.wav");
    fx_point = LoadSound("assets/point.wav");
    fx_level = LoadSound("assets/level.wav");
    font = LoadFontEx("assets/font_silkscreen/slkscr.ttf", 36, 0, 98);
    SetTextureFilter(font.texture, 0);

    player_init();
    paddle_speed = PADDLE_SPEED;

    npc_init();

    ball_init();

    score_size = MeasureTextEx(font, "00", SCORE_TEXT_SIZE, 0);
    level_text_size = MeasureTextEx(font, "L 0", 24, -4);

    player_score = 0;
    npc_score = 0;
    level = 1;
}

void scene_gameplay_update(float delta_time)
{
    input_update();
    player_update(delta_time);
    npc_update(delta_time);
    ball_update(delta_time);
};

void input_update()
{
    if (IsKeyDown(KEY_UP))
    {
        player.velocity.y = -paddle_speed;
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        player.velocity.y = paddle_speed;
    }
}

void event_wall()
{
    PlaySound(fx_bounce2);
}

void event_paddle_bounce()
{
    PlaySound(fx_bounce1);
}

void event_player_score()
{
    PlaySound(fx_point);
    player_score++;
    if (player_score > POINTS_PER_LEVEL)
    {
        level++;
        player_score = 0;
        npc_score = 0;
        PlaySound(fx_level);
        new_level();
    }
    ball_reset(true);
}

void event_npc_score()
{
    PlaySound(fx_point);
    npc_score++;
    check_gameover();
    ball_reset(false);
}

void new_level() // TODO
{
    ball_speed *= 1.10;
    //npc_speed_factor *= 1.15;
    paddle_speed *= 1.10;
}

void check_gameover()
{
    if (npc_score > POINTS_PER_LEVEL)
    {
        scene_transition_init(SCENE_GAMEPLAY, SCENE_GAMEOVER);
    }
}

void scene_gameplay_draw()
{
    DrawTextureRec(texture_atlas, FIELD_FRAME_RECT, (Vector2){0, 0}, WHITE);
    DrawTextEx(font, TextFormat("L %i", level), (Vector2){320 - level_text_size.x / 2, 18}, 24, -4, LEVEL_TEXT_COLOR);
    DrawTextEx(font, TextFormat("%02i", player_score), (Vector2){320 - 40 - score_size.x, score_text_y}, SCORE_TEXT_SIZE, 0, SCORE_TEXT_COLOR);
    DrawTextEx(font, TextFormat("%02i", npc_score), (Vector2){320 + 40, score_text_y}, SCORE_TEXT_SIZE, 0, SCORE_TEXT_COLOR);
    DrawTextureRec(texture_atlas, player.frame_rect, (Vector2){player.bounds.x, player.bounds.y}, WHITE);
    DrawTextureRec(texture_atlas, npc.frame_rect, (Vector2){npc.bounds.x, npc.bounds.y}, WHITE);
    DrawTextureRec(texture_atlas, ball.frame_rect, (Vector2){ball.bounds.x, ball.bounds.y}, WHITE);
};
void scene_gameplay_destroy()
{
    UnloadSound(fx_bounce1);
    UnloadSound(fx_bounce2);
    UnloadSound(fx_point);
    UnloadSound(fx_level);
    UnloadFont(font);
};