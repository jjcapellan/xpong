#include "defs.h"
#include "gameplay.h"

//
// LOCAL VARIABLES
//

Vector2 score_size;
Vector2 level_text_size;
int npc_score;
float paddle_speed;
int player_score;
int score_text_y = (SCREEN_HEIGHT / 2) - (SCORE_TEXT_SIZE / 2);
int level;
bool is_pause = false;

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

    player_init();
    paddle_speed = PADDLE_SPEED;

    npc_init();

    ball_init();

    particles_init();

    score_size = MeasureTextEx(font, "00", SCORE_TEXT_SIZE, 0);
    level_text_size = MeasureTextEx(font, "L 0", 24, -4);

    player_score = 0;
    npc_score = 0;
    level = 1;
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
    // NPC SPEED
    if (IsKeyReleased(KEY_S))
    {
        npc_speed += 10;
    }
    else if (IsKeyReleased(KEY_A))
    {
        npc_speed -= 10;
    }
    if (npc_speed > 1000)
        npc_speed = 1000;
    if (npc_speed < 100)
        npc_speed = 100;

    // NPC TIME REACTION
    if (IsKeyReleased(KEY_F))
    {
        npc_max_time_react += 0.1f;
    }
    else if (IsKeyReleased(KEY_D))
    {
        npc_max_time_react -= 0.1;
    }
    if (npc_max_time_react > 1.2f)
        npc_max_time_react = 1.2f;
    if (npc_max_time_react < 0)
        npc_max_time_react = 0;

    // BALL SPEED
    if (IsKeyReleased(KEY_K))
    {
        ball_speed += 10;
    }
    else if (IsKeyReleased(KEY_J))
    {
        ball_speed -= 10;
    }
    if (ball_speed > 2000)
        ball_speed = 2000;
    if (ball_speed < 100)
        ball_speed = 100;
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
    // ball_speed *= 1.10;
    // npc_speed_factor *= 1.15;
    // paddle_speed *= 1.10;

    // TODO
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

    DrawTextEx(font, TextFormat("L %i", level), (Vector2){320 - level_text_size.x / 2, 18}, 24, -4, LEVEL_TEXT_COLOR);
    DrawTextEx(font, TextFormat("%02i", player_score), (Vector2){320 - 40 - score_size.x, score_text_y}, SCORE_TEXT_SIZE, 0, SCORE_TEXT_COLOR);
    DrawTextEx(font, TextFormat("%02i", npc_score), (Vector2){320 + 40, score_text_y}, SCORE_TEXT_SIZE, 0, SCORE_TEXT_COLOR);
#ifdef DEBUG
    DrawText(TextFormat("(A-S)    npc speed: %.0f px/sec", npc_speed), 265, 40, 18, SCORE_TEXT_COLOR);
    DrawText(TextFormat("(D-F)    npc max time reaction: %.01f secs.", npc_max_time_react), 265, 60, 18, SCORE_TEXT_COLOR);
    DrawText(TextFormat("(J-K)    ball speed: %.0f px/sec", ball_speed), 265, 100, 18, SCORE_TEXT_COLOR);
    DrawText(TextFormat("DEBUG MODE", ball_speed), 265 - 120, 480 - 40, 18, LEVEL_TEXT_COLOR);
#endif
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