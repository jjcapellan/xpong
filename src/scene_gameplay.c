#ifdef DEBUG
#include <stdio.h>
#endif
#include "defs.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>

// Local types

struct Entity
{
    Rectangle frameRect;
    Rectangle bounds;
    Vector2 velocity; // px/sec
};

// Local variables

Rectangle paddle_frame_rect;
Rectangle ball_frame_rect;
Rectangle field_frame_rect;
struct Entity paddle1;
struct Entity paddle2;
struct Entity ball;
float npcSpeedFactor;

// Local functions
void input_update();
void paddle1_update(float deltaTime);
void paddle2_update(float deltaTime);
void ball_update(float deltaTime);
void ball_collision_paddle1();
float vector2_get_angle(Vector2 v);
void vector2_set_angle(Vector2 *v, float angle, float length);

// Resources

Sound fx_bounce;
Texture2D texture_atlas; // Global

void scene_gameplay_init()
{
    fx_bounce = LoadSound("assets/bound.wav");

    paddle_frame_rect = (Rectangle){0, 480, 12, 72};
    ball_frame_rect = (Rectangle){12, 480, 12, 12};
    field_frame_rect = (Rectangle){0, 0, 640, 480};
    int paddle_width = paddle_frame_rect.width;
    int paddle_height = paddle_frame_rect.height;
    int ball_size = ball_frame_rect.width;

    paddle1.frameRect = paddle_frame_rect;
    paddle1.bounds = (Rectangle){PADDLE_H_MARGIN, SCREEN_HEIGHT / 2 - paddle_height / 2, paddle_width, paddle_height};
    paddle1.velocity = (Vector2){0, 0};

    paddle2.frameRect = paddle_frame_rect;
    paddle2.bounds = (Rectangle){SCREEN_WIDTH - (paddle_width + PADDLE_H_MARGIN), SCREEN_HEIGHT / 2 - paddle_height / 2, paddle_width, paddle_height};
    paddle2.velocity = (Vector2){0, 0};

    ball.frameRect = ball_frame_rect;
    ball.bounds = (Rectangle){SCREEN_WIDTH / 2 - ball_size / 2, SCREEN_HEIGHT / 2 - ball_size / 2, ball_size, ball_size};
    Vector2 v = (Vector2){BALL_SPEED, 0};
    float angle = (float)GetRandomValue(135, 225) * DEG2RAD;
    ball.velocity = Vector2Rotate(v, angle);

    npcSpeedFactor = (NPC_MAX_SPEED_FACTOR - NPC_MIN_SPEED_FACTOR) / (paddle2.bounds.x - paddle1.bounds.x);
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

    paddle2.bounds.y += paddle2.velocity.y * deltaTime * npcSpeedFactor * ball.bounds.x;

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
    if ((ball.bounds.y + ball.bounds.height) > WORLD_BOUNDS.y + WORLD_BOUNDS.height)
    {
        ball.bounds.y = WORLD_BOUNDS.y + WORLD_BOUNDS.height - ball.bounds.height;
        ball.velocity.y *= -1;
    }
    if (ball.bounds.y < WORLD_BOUNDS.y)
    {
        ball.bounds.y = WORLD_BOUNDS.y;
        ball.velocity.y *= -1;
    }

    // PADDLE COLLISION
    if (ball.bounds.x < paddle1.bounds.x + paddle1.bounds.width || ball.bounds.x + ball.bounds.width > paddle2.bounds.x)
    {
        if (CheckCollisionRecs(ball.bounds, paddle1.bounds))
        {
            ball_collision_paddle1();
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
            ball.bounds.x = paddle2.bounds.x - paddle2.bounds.width;
            ball.velocity.x *= -1;
        }
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

void ball_collision_paddle1(){
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

void scene_gameplay_draw()
{
    BeginDrawing();
    ClearBackground(WHITE);
    DrawTextureRec(texture_atlas, field_frame_rect, (Vector2){0, 0}, WHITE);
    DrawTextureRec(texture_atlas, paddle1.frameRect, (Vector2){paddle1.bounds.x, paddle1.bounds.y}, WHITE);
    DrawTextureRec(texture_atlas, paddle2.frameRect, (Vector2){paddle2.bounds.x, paddle2.bounds.y}, WHITE);
    DrawTextureRec(texture_atlas, ball.frameRect, (Vector2){ball.bounds.x, ball.bounds.y}, WHITE);
    EndDrawing();
};
void scene_gameplay_destroy()
{
    UnloadSound(fx_bounce);
};