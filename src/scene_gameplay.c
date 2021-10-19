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

// Local functions
void input_update();
void paddle1_update(float deltaTime);
void ball_update(float deltaTime);

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
}

void scene_gameplay_update(float deltaTime)
{
    input_update();
    paddle1_update(deltaTime);
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

void ball_update(float deltaTime)
{
    ball.bounds.x += ball.velocity.x * deltaTime;
    ball.bounds.y += ball.velocity.y * deltaTime;

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
    if (ball.bounds.x < paddle1.bounds.x + paddle1.bounds.width || ball.bounds.x + ball.bounds.width > paddle2.bounds.x)
    {
        if (CheckCollisionRecs(ball.bounds, paddle1.bounds))
        {
            int distToCenter = (paddle1.bounds.y + paddle1.bounds.height / 2) - (ball.bounds.y + ball.bounds.height / 2);

            ball.bounds.x = paddle1.bounds.x + paddle1.bounds.width;
            ball.velocity.x *= -1;

            float angle1 = (-PADDLE_MAX_ANGLE * DEG2RAD * distToCenter) / (paddle1.bounds.height / 2);
            bool isGoingUp = (ball.velocity.y < 0);

            ball.velocity = Vector2Rotate(ball.velocity, angle1);
            float angle2 = atan2f(ball.velocity.y, ball.velocity.x) * RAD2DEG;
            if (angle2 > BALL_MAX_ANGLE || angle2 < -BALL_MAX_ANGLE)
            {
                float angle = BALL_MAX_ANGLE * DEG2RAD;
                if (isGoingUp)
                {
                    angle *= -1;
                }
                ball.velocity.x = cosf(angle) * BALL_SPEED;
                ball.velocity.y = sinf(angle) * BALL_SPEED;
            }
        }

        if (ball.bounds.x > 630)
        {
            ball.bounds.x -= 2;
            ball.velocity.x *= -1;
        }
        /*if (CheckCollisionRecs(ball.bounds, paddle2.bounds))
        {
            ball.bounds.x = paddle2.bounds.x - paddle2.bounds.width;
            ball.velocity.x *= -1;
        }*/
    }
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