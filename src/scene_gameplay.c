#include "defs.h"
#include "gameplay.h"

//
// LOCAL VARIABLES
//

Vector2 score_size;
Vector2 level_text_size;
int npc_score;
int player_score;
int score_text_y = (SCREEN_HEIGHT / 2) - (SCORE_TEXT_SIZE / 2);
int level = 0;
bool is_pause = false;
Level levels[8] = {0};

//
// LOCAL FUNCTIONS
//

void input_update();
void check_gameover();
void new_level();
void init_levels();

//
// RESOURCES
//

Sound fx_bounce1;
Sound fx_bounce2;
Sound fx_point;
Sound fx_level;
Font font;

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

    HideCursor();
    DisableCursor();

    init_levels();

    player_init();

    npc_init();

    ball_init();

    particles_init();

    score_size = MeasureTextEx(font, "00", SCORE_TEXT_SIZE, 0);
    level_text_size = MeasureTextEx(font, "L 0", 24, -4);

    player_score = 0;
    npc_score = 0;
    level = 0;
}

void scene_gameplay_update(float delta_time)
{
    input_update();
    if (is_pause == false)
    {
        player_update(delta_time);
        npc_update(delta_time);
        ball_update(delta_time);
        particles_update(delta_time);
    }
};

void input_update()
{

    if (IsKeyReleased(KEY_P))
    {
        if (is_pause == false)
        {
            is_pause = true;
        }
        else
        {
            is_pause = false;
        }
    }
#ifdef DEBUG
    if (IsKeyReleased(KEY_L))
    {
        new_level();
    }
#endif
}

void event_wall()
{
    PlaySound(fx_bounce2);
}

void event_paddle_bounce()
{
    PlaySound(fx_bounce1);
    particles_reset((Vector2){ball.bounds.x, ball.bounds.y});
}

void event_player_score()
{
    PlaySound(fx_point);
    player_score++;
    if (player_score > POINTS_PER_LEVEL)
    {
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

void init_levels()
{
    levels[0] = (Level){NPC_ERRORS_LEVEL_1, BALL_SPEED_LEVEL_1};
    levels[1] = (Level){NPC_ERRORS_LEVEL_2, BALL_SPEED_LEVEL_2};
    levels[2] = (Level){NPC_ERRORS_LEVEL_3, BALL_SPEED_LEVEL_3};
    levels[3] = (Level){NPC_ERRORS_LEVEL_4, BALL_SPEED_LEVEL_4};
    levels[4] = (Level){NPC_ERRORS_LEVEL_5, BALL_SPEED_LEVEL_5};
    levels[5] = (Level){NPC_ERRORS_LEVEL_6, BALL_SPEED_LEVEL_6};
    levels[6] = (Level){NPC_ERRORS_LEVEL_7, BALL_SPEED_LEVEL_7};
    levels[7] = (Level){NPC_ERRORS_LEVEL_8, BALL_SPEED_LEVEL_8};
}

void new_level()
{
    player_score = 0;
    npc_score = 0;
    PlaySound(fx_level);
    level++;
    npc_max_error = levels[level].npc_max_error;
    ball_speed = levels[level].ball_speed;
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
    for (int i = 0; i < 6; i++)
    {
        if (particles[i].alive == true)
        {
            DrawTexturePro(texture_atlas, PARTICLE_FRAME_RECT, (Rectangle){particles[i].position.x, particles[i].position.y, 6 * particles[i].scale, 6 * particles[i].scale}, (Vector2){0.5, 0.5}, particles[i].rotation, WHITE);
        }
    }

    DrawTextEx(font, TextFormat("L %i", level + 1), (Vector2){320 - level_text_size.x / 2, 18}, 24, -4, LEVEL_TEXT_COLOR);
    DrawTextEx(font, TextFormat("%02i", player_score), (Vector2){320 - 40 - score_size.x, score_text_y}, SCORE_TEXT_SIZE, 0, SCORE_TEXT_COLOR);
    DrawTextEx(font, TextFormat("%02i", npc_score), (Vector2){320 + 40, score_text_y}, SCORE_TEXT_SIZE, 0, SCORE_TEXT_COLOR);

    player_draw();
    npc_draw();
    ball_draw();
};

void scene_gameplay_destroy()
{
    UnloadSound(fx_bounce1);
    UnloadSound(fx_bounce2);
    UnloadSound(fx_point);
    UnloadSound(fx_level);
    UnloadFont(font);
};