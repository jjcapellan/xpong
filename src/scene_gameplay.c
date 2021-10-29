#ifdef DEBUG
#include <stdio.h>
#endif
#include "defs.h"
#include "raylib.h"
#include <math.h>

// Local types

struct Entity
{
    Rectangle frame_rect;
    Rectangle bounds;
    Vector2 velocity; // px/sec
};

// Local variables

Rectangle paddle_frame_rect = PADDLE_FRAME_RECT;
Rectangle ball_frame_rect = BALL_FRAME_RECT;
Rectangle field_frame_rect = (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_WIDTH};
Rectangle world_bounds = WORLD_BOUNDS;
int player_score;
int npc_score;
int score_text_y = (SCREEN_HEIGHT / 2) - (SCORE_TEXT_SIZE / 2);

struct Entity paddle1;
struct Entity paddle2;
struct Entity ball;
float npc_speed_factor;
Vector2 score_size;

// Local functions
void input_update();
void paddle1_update(float deltaTime);
void paddle2_update(float deltaTime);
void ball_update(float deltaTime);
void ball_collision_paddle1();
void ball_collision_paddle2();
void ball_reset(bool isPlayer);
float vector2_get_angle(Vector2 v);
void vector2_set_angle(Vector2 *v, float angle, float length);
void check_gameover();

// Resources

Sound fx_bounce1;
Sound fx_bounce2;
Sound fx_point;
Font font;
Texture2D texture_atlas; // Global

void scene_gameplay_init()
{
    fx_bounce1 = LoadSound("assets/bounce1.wav");
    fx_bounce2 = LoadSound("assets/bounce2.wav");
    fx_point = LoadSound("assets/point.wav");
    font = LoadFontEx("assets/font_silkscreen/slkscr.ttf", 36, 0, 98);
    SetTextureFilter(font.texture, 0);

    int paddle_width = paddle_frame_rect.width;
    int paddle_height = paddle_frame_rect.height;

    paddle1.frame_rect = paddle_frame_rect;
    paddle1.bounds = (Rectangle){PADDLE_H_MARGIN, SCREEN_HEIGHT / 2 - paddle_height / 2, paddle_width, paddle_height};
    paddle1.velocity = (Vector2){0, 0};

    paddle2.frame_rect = paddle_frame_rect;
    paddle2.bounds = (Rectangle){SCREEN_WIDTH - (paddle_width + PADDLE_H_MARGIN), SCREEN_HEIGHT / 2 - paddle_height / 2, paddle_width, paddle_height};
    paddle2.velocity = (Vector2){0, 0};

    ball_reset(false);

    npc_speed_factor = (NPC_MAX_SPEED_FACTOR - NPC_MIN_SPEED_FACTOR) / (paddle2.bounds.x - paddle1.bounds.x);
    //score_size = MeasureText("00", SCORE_TEXT_SIZE);
    score_size = MeasureTextEx(font, "00", SCORE_TEXT_SIZE, 0);

    player_score = 0;
    npc_score = 0;
}

void scene_gameplay_update(float deltaTime)
{
    input_update();
    paddle1_update(deltaTime);
    paddle2_update(deltaTime);
    ball_update(deltaTime);
};

void input_update()
{
    if (IsKeyDown(KEY_UP))
    {
        paddle1.velocity.y = -PADDLE_SPEED;
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        paddle1.velocity.y = PADDLE_SPEED;
    }
}

void paddle1_update(float deltaTime)
{
    paddle1.bounds.y += paddle1.velocity.y * deltaTime;

    if (paddle1.bounds.y < 4)
    {
        paddle1.bounds.y = 4;
    }
    else if (paddle1.bounds.y > (SCREEN_HEIGHT - 4 - paddle1.bounds.height))
    {
        paddle1.bounds.y = SCREEN_HEIGHT - 4 - paddle1.bounds.height;
    }

    paddle1.velocity.y = 0;
}

void paddle2_update(float deltaTime)
{
    if (ball.velocity.x > 0)
    {
        if ((ball.bounds.y + ball.bounds.height / 2) < (paddle2.bounds.y + paddle2.bounds.height / 2))
        {
            paddle2.velocity.y = -PADDLE_SPEED;
        }
        else
        {
            paddle2.velocity.y = PADDLE_SPEED;
        }
    }

    paddle2.bounds.y += paddle2.velocity.y * deltaTime * npc_speed_factor * ball.bounds.x;

    if (paddle2.bounds.y < 4)
    {
        paddle2.bounds.y = 4;
    }
    else if (paddle2.bounds.y > (SCREEN_HEIGHT - 4 - paddle2.bounds.height))
    {
        paddle2.bounds.y = SCREEN_HEIGHT - 4 - paddle2.bounds.height;
    }

    paddle2.velocity.y = 0;
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
    if (ball.bounds.x < paddle1.bounds.x + paddle1.bounds.width || ball.bounds.x + ball.bounds.width > paddle2.bounds.x)
    {
        if (CheckCollisionRecs(ball.bounds, paddle1.bounds))
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

        if (CheckCollisionRecs(ball.bounds, paddle2.bounds))
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
        ball_reset(true);
    }
}

void check_gameover()
{
    if (npc_score > 11)
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
    int distToCenter = (paddle1.bounds.y + paddle1.bounds.height / 2) - (ball.bounds.y + ball.bounds.height / 2);

    ball.bounds.x = paddle1.bounds.x + paddle1.bounds.width;
    ball.velocity.x *= -1;

    float angle0 = vector2_get_angle(ball.velocity);
    float dAngle = (-PADDLE_MAX_ANGLE * distToCenter) / (paddle1.bounds.height / 2);
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
    vector2_set_angle(&ball.velocity, angle1 * DEG2RAD, BALL_SPEED);
}

void ball_collision_paddle2()
{
    int distToCenter = (paddle2.bounds.y + paddle2.bounds.height / 2) - (ball.bounds.y + ball.bounds.height / 2);

    ball.bounds.x = paddle2.bounds.x - ball.bounds.width;
    ball.velocity.x *= -1;

    float angle0 = vector2_get_angle(ball.velocity);
    float dAngle = (PADDLE_MAX_ANGLE * distToCenter) / (paddle2.bounds.height / 2);
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
    vector2_set_angle(&ball.velocity, angle1 * DEG2RAD, BALL_SPEED);
}

void ball_reset(bool isPlayer)
{
    ball.frame_rect = ball_frame_rect;
    int ball_size = ball_frame_rect.width;
    ball.bounds = (Rectangle){SCREEN_WIDTH / 2 - ball_size / 2, SCREEN_HEIGHT / 2 - ball_size / 2, ball_size, ball_size};
    float angle = (float)GetRandomValue(135, 225) * DEG2RAD;
    vector2_set_angle(&ball.velocity, angle, BALL_SPEED / 2);
    if (isPlayer)
        ball.velocity.x *= -1;
}

void scene_gameplay_draw()
{
    DrawTextureRec(texture_atlas, field_frame_rect, (Vector2){0, 0}, WHITE);
    DrawTextEx(font, TextFormat("%02i", player_score), (Vector2){320 - 40 - score_size.x, score_text_y}, SCORE_TEXT_SIZE, 0, SCORE_TEXT_COLOR);
    DrawTextEx(font, TextFormat("%02i", npc_score), (Vector2){320 + 40, score_text_y}, SCORE_TEXT_SIZE, 0, SCORE_TEXT_COLOR);
    DrawTextureRec(texture_atlas, paddle1.frame_rect, (Vector2){paddle1.bounds.x, paddle1.bounds.y}, WHITE);
    DrawTextureRec(texture_atlas, paddle2.frame_rect, (Vector2){paddle2.bounds.x, paddle2.bounds.y}, WHITE);
    DrawTextureRec(texture_atlas, ball.frame_rect, (Vector2){ball.bounds.x, ball.bounds.y}, WHITE);
};
void scene_gameplay_destroy()
{
    UnloadSound(fx_bounce1);
    UnloadSound(fx_bounce2);
    UnloadSound(fx_point);
    UnloadFont(font);
};