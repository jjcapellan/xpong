#ifdef DEBUG
#include <stdio.h>
#endif
#include "defs.h"
#include "gameplay.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>

// Local variables

Rectangle paddle_frame_rect = PADDLE_FRAME_RECT;
Rectangle ball_frame_rect = BALL_FRAME_RECT;
Rectangle field_frame_rect = (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_WIDTH};
Rectangle world_bounds = WORLD_BOUNDS;

Entity player; // Global
Entity npc;  // Global
int npc_score;
float paddle_speed;

Entity ball;
float ball_prev_vel_x;
int ball_speed;

int player_score;
int score_text_y = (SCREEN_HEIGHT / 2) - (SCORE_TEXT_SIZE / 2);
int level;

Vector2 score_size;
Vector2 level_text_size;

// Local functions
void input_update();
void ball_update(float deltaTime);
void ball_collision_paddle1();
void ball_collision_paddle2();
void ball_reset(bool isPlayer);
float vector2_get_angle(Vector2 v);
void vector2_set_angle(Vector2 *v, float angle, float length);
void check_gameover();
void new_level();

// Resources

Sound fx_bounce1;
Sound fx_bounce2;
Sound fx_point;
Sound fx_level;
Font font;
Texture2D texture_atlas; // Global

void scene_gameplay_init()
{
    fx_bounce1 = LoadSound("assets/bounce1.wav");
    fx_bounce2 = LoadSound("assets/bounce2.wav");
    fx_point = LoadSound("assets/point.wav");
    fx_level = LoadSound("assets/level.wav");
    font = LoadFontEx("assets/font_silkscreen/slkscr.ttf", 36, 0, 98);
    SetTextureFilter(font.texture, 0);

    int paddle_width = paddle_frame_rect.width;
    int paddle_height = paddle_frame_rect.height;

    player_init();
    
    npc_init();

    paddle_speed = PADDLE_SPEED;
    ball_speed = BALL_SPEED;

    ball_reset(false);
    ball_prev_vel_x = ball.velocity.x;

    score_size = MeasureTextEx(font, "00", SCORE_TEXT_SIZE, 0);
    level_text_size = MeasureTextEx(font, "L 0", 24, -4);

    player_score = 0;
    npc_score = 0;
    level = 1;
}

void scene_gameplay_update(float deltaTime)
{
    input_update();
    player_update(deltaTime);
    npc_update(deltaTime);
    ball_update(deltaTime);
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

void paddle1_update(float deltaTime)
{
    player.bounds.y += player.velocity.y * deltaTime;

    if (player.bounds.y < 4)
    {
        player.bounds.y = 4;
    }
    else if (player.bounds.y > (SCREEN_HEIGHT - 4 - player.bounds.height))
    {
        player.bounds.y = SCREEN_HEIGHT - 4 - player.bounds.height;
    }

    player.velocity.y = 0;
}

void ball_update(float deltaTime)
{
    ball.bounds.x += ball.velocity.x * deltaTime;
    ball.bounds.y += ball.velocity.y * deltaTime;

    // TOP/BOTTOM COLLISION
    if ((ball.bounds.y + ball.bounds.height) > world_bounds.y + world_bounds.height)
    {
        ball.bounds.y = world_bounds.y + world_bounds.height - ball.bounds.height;
        ball.velocity.y *= -1;
        PlaySound(fx_bounce2);
    }
    if (ball.bounds.y < world_bounds.y)
    {
        ball.bounds.y = world_bounds.y;
        ball.velocity.y *= -1;
        PlaySound(fx_bounce2);
    }

    // PADDLE COLLISION
    if (ball.bounds.x < player.bounds.x + player.bounds.width || ball.bounds.x + ball.bounds.width > npc.bounds.x)
    {
        if (CheckCollisionRecs(ball.bounds, player.bounds))
        {
            ball_collision_paddle1();
            PlaySound(fx_bounce1);
        }

#ifdef DEBUG
        if (ball.bounds.x > 630)
        {
            ball.bounds.x -= 2;
            ball.velocity.x *= -1;
        }
#endif

        if (CheckCollisionRecs(ball.bounds, npc.bounds))
        {
            ball_collision_paddle2();
            PlaySound(fx_bounce1);
        }
    }

    // SCORE EVENTS
    if (ball.bounds.x < 0)
    {
        PlaySound(fx_point);
        npc_score++;
        check_gameover();
        ball_reset(false);
    }
    if (ball.bounds.x > SCREEN_WIDTH)
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
}

void new_level()
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

float vector2_get_angle(Vector2 v)
{
    float angle = atan2f(v.y, v.x) * RAD2DEG;
    if (angle < 0)
    {
        angle += 360;
    }
    return angle;
}

// Angle in radians
void vector2_set_angle(Vector2 *v, float angle, float length)
{
    v->x = cosf(angle) * length;
    v->y = sinf(angle) * length;
}

void ball_collision_paddle1()
{
    int distToCenter = (player.bounds.y + player.bounds.height / 2) - (ball.bounds.y + ball.bounds.height / 2);

    ball.bounds.x = player.bounds.x + player.bounds.width;
    ball.velocity.x *= -1;

    float angle0 = vector2_get_angle(ball.velocity);
    float dAngle = (-PADDLE_MAX_ANGLE * distToCenter) / (player.bounds.height / 2);
    float angle1 = angle0 + dAngle;

    if (angle1 > BALL_MAX_ANGLE && angle1 < 360 - BALL_MAX_ANGLE)
    {
        if (angle0 < 90)
        {
            angle1 = BALL_MAX_ANGLE;
        }
        else
        {
            angle1 = 360 - BALL_MAX_ANGLE;
        }
    }
    vector2_set_angle(&ball.velocity, angle1 * DEG2RAD, ball_speed);
}

void ball_collision_paddle2()
{
    int distToCenter = (npc.bounds.y + npc.bounds.height / 2) - (ball.bounds.y + ball.bounds.height / 2);

    ball.bounds.x = npc.bounds.x - ball.bounds.width;
    ball.velocity.x *= -1;

    float angle0 = vector2_get_angle(ball.velocity);
    float dAngle = (PADDLE_MAX_ANGLE * distToCenter) / (npc.bounds.height / 2);
    float angle1 = angle0 + dAngle;

    if (angle1 < 180 - BALL_MAX_ANGLE || angle1 > 180 + BALL_MAX_ANGLE)
    {
        if (angle0 < 180)
        {
            angle1 = 180 - BALL_MAX_ANGLE;
        }
        else
        {
            angle1 = 180 + BALL_MAX_ANGLE;
        }
    }
    vector2_set_angle(&ball.velocity, angle1 * DEG2RAD, ball_speed);
}

void ball_reset(bool isPlayer)
{
    ball.frame_rect = ball_frame_rect;
    int ball_size = ball_frame_rect.width;
    ball.bounds = (Rectangle){SCREEN_WIDTH / 2 - ball_size / 2, SCREEN_HEIGHT / 2 - ball_size / 2, ball_size, ball_size};
    float angle = (float)GetRandomValue(135, 225) * DEG2RAD;
    vector2_set_angle(&ball.velocity, angle, BALL_SPEED_START);
    if (isPlayer)
        ball.velocity.x *= -1;

    npc_reset();
}

void scene_gameplay_draw()
{
    DrawTextureRec(texture_atlas, field_frame_rect, (Vector2){0, 0}, WHITE);
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