#include "defs.h"
#include "gameplay.h"
#include <stdlib.h>
#include <stdio.h>

//
// GLOBALS
//

Entity npc = (Entity){0};
Entity ball;

//
// LOCAL TYPES
//

typedef enum Npc_states
{
    THINKING,
    READY,
    IDLE,
    UP,
    DOWN
} Npc_state;

//
// LOCAL FUNCTIONS
//

void npc_update(float delta_time);
void check_npc_event(float delta_time);
void npc_set_speed_factor(float min, float max);
float entity_get_y_center(Entity entity);
float npc_get_time_react();

//
// LOCAL VARIABLES
//

float npc_speed_factor;
float npc_speed;
float npc_time_react;
float npc_min_time_react;
float npc_max_time_react;
float npc_ball_prev_y_offset; // difference (ball center y - paddle center y)
float ball_prev_vel_x;
float ball_prev_vel_y;
Npc_state npc_state;

//
// FUNCTIONS
//

void npc_init()
{
    npc.frame_rect = PADDLE_FRAME_RECT;
    float height = npc.frame_rect.height;
    float width = npc.frame_rect.width;
    npc.bounds = (Rectangle){SCREEN_WIDTH - (width + PADDLE_H_MARGIN), SCREEN_HEIGHT / 2 - height / 2, width, height};
    npc.velocity = (Vector2){0, 0};

    // velocity_multiplier = min_multiplier + npc_speed_factor * distance_ball_npc
    npc_speed_factor = (NPC_MIN_SPEED_FACTOR - NPC_MAX_SPEED_FACTOR) / (npc.bounds.x - PADDLE_H_MARGIN);
    npc_speed = PADDLE_SPEED;

    npc_state = THINKING;
    npc_min_time_react = NPC_MIN_TIME_REACTION;
    npc_max_time_react = NPC_MAX_TIME_REACTION;
    npc_time_react = npc_get_time_react();
    npc_ball_prev_y_offset = 0;
    ball_prev_vel_x = 0;
    ball_prev_vel_x = 0;
}

void npc_update(float delta_time)
{
    check_npc_event(delta_time);
    if (npc_state == THINKING)
    {
        npc.velocity.y = 0;
    }

    if (npc_state == READY)
    {
        if (ball.velocity.x < 0)
        {
            npc_state = IDLE;
        }
        else
        {
            float y_offset = entity_get_y_center(ball) - entity_get_y_center(npc);
            if (y_offset < 0)
            {
                npc_state = UP;
            }
            else if (y_offset > 0)
            {
                npc_state = DOWN;
            }
            else
            {
                npc_state = IDLE;
            }
        }
    }

    float multiplier = 1;

    switch (npc_state)
    {
    case UP:
        multiplier = NPC_MAX_SPEED_FACTOR + npc_speed_factor * (npc.bounds.x - ball.bounds.x);
        npc.velocity.y = -PADDLE_SPEED;
        break;
    case DOWN:
        multiplier = NPC_MAX_SPEED_FACTOR + npc_speed_factor * (npc.bounds.x - ball.bounds.x);
        npc.velocity.y = PADDLE_SPEED;

        break;
    case IDLE:
        npc.velocity.y = 0;
        break;
    }

    npc.velocity.y *= multiplier;
    npc.bounds.y += npc.velocity.y * delta_time;

    if (npc.bounds.y < 4)
    {
        npc.bounds.y = 4;
        npc.velocity.y = 0;
    }
    else if (npc.bounds.y > (SCREEN_HEIGHT - 4 - npc.bounds.height))
    {
        npc.bounds.y = SCREEN_HEIGHT - 4 - npc.bounds.height;
        npc.velocity.y = 0;
    }
}

void check_npc_event(float delta_time)
{
    float current_npc_ball_y_offset = entity_get_y_center(ball) - entity_get_y_center(npc);
    if (npc_state == THINKING)
    {
        npc_time_react -= delta_time;
    }
    if (npc_time_react < 0)
    {
        npc_time_react = 0;
        npc_state = READY;
        return;
    }
    if (ball_prev_vel_y * ball.velocity.y < 0)
    {
        npc_state = THINKING;
        npc_time_react = npc_get_time_react();
    }
    if (ball_prev_vel_x * ball.velocity.x < 0)
    {
        npc_state = THINKING;
        npc_time_react = npc_get_time_react() + 0.1;
    }
    if (npc_ball_prev_y_offset * current_npc_ball_y_offset < 0)
    {
        if ((npc.bounds.x - ball.bounds.x) > npc.bounds.height)
        {
            npc_state = THINKING;
            npc_time_react = 0.1;
        }
        else
        {
            npc_state = READY;
        }
    }

    npc_ball_prev_y_offset = current_npc_ball_y_offset;
    ball_prev_vel_x = ball.velocity.x;
    ball_prev_vel_y = ball.velocity.y;
}

float entity_get_y_center(Entity entity)
{
    return entity.bounds.y + entity.bounds.height;
}

float npc_get_time_react()
{
    return (drand48() * (npc_max_time_react - npc_min_time_react) + npc_min_time_react);
}

void npc_set_speed_factor(float min, float max)
{
    // TODO
}

void npc_reset()
{
    npc_state = THINKING;
    npc_time_react = npc_get_time_react();
}