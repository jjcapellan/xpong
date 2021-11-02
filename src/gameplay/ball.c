#include "defs.h"
#include "gameplay.h"

//
//LOCAL FUNCTIONS
//

float vector2_get_angle(Vector2 v);
void vector2_set_angle(Vector2 *v, float angle, float length);
void collision_ball_player();
void collision_ball_npc();

//
// GLOBALS
//

Entity ball = {0};
Vector2 ball_destination = {0};
Entity player;
Entity npc;

//
// LOCAL VARIABLES
//

float ball_speed = 0;
Rectangle world_bounds = WORLD_BOUNDS;

//
// FUNCTIONS
//

void ball_init()
{
    ball_speed = BALL_SPEED;
    ball_reset(false);
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
        set_ball_destination();
        event_wall();
    }
    if (ball.bounds.y < world_bounds.y)
    {
        ball.bounds.y = world_bounds.y;
        ball.velocity.y *= -1;
        set_ball_destination();
        event_wall();
    }

    // PADDLE COLLISION
    if (ball.bounds.x < player.bounds.x + player.bounds.width || ball.bounds.x + ball.bounds.width > npc.bounds.x)
    {
        if (CheckCollisionRecs(ball.bounds, player.bounds))
        {
            collision_ball_player();
            event_paddle_bounce();
        }

        if (CheckCollisionRecs(ball.bounds, npc.bounds))
        {
            collision_ball_npc();
            event_paddle_bounce();
        }
    }

    // SCORE EVENTS
    if (ball.bounds.x < 0)
    {
        event_npc_score();
    }
    if (ball.bounds.x > SCREEN_WIDTH)
    {
        event_player_score();
    }
}

void collision_ball_player()
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
    set_ball_destination();
}

void collision_ball_npc()
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

// Calcs ball.y for ball.x = npc.x if there were not wall bounces
void set_ball_destination()
{
    float m, x, y;
    m = ball.velocity.y / ball.velocity.x;
    x = world_bounds.x + world_bounds.width - PADDLE_H_MARGIN;
    y = ball.bounds.y - m * ball.bounds.x + m * x;

    ball_destination.x = x;
    ball_destination.y = y;
}

void ball_reset(bool isPlayer)
{
    ball.frame_rect = BALL_FRAME_RECT;
    int ball_size = ball.frame_rect.width;
    ball.bounds = (Rectangle){SCREEN_WIDTH / 2 - ball_size / 2, SCREEN_HEIGHT / 2 - ball_size / 2, ball_size, ball_size};
    float angle = (float)GetRandomValue(135, 225) * DEG2RAD;
    vector2_set_angle(&ball.velocity, angle, BALL_SPEED_START);
    if (isPlayer)
    {
        ball.velocity.x *= -1;
        set_ball_destination();
        npc_set_current_target();
        npc_reset();
    }
    else
    {
        set_ball_destination();
        npc_set_current_target();
        npc_reset();
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
