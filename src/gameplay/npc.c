#include "defs.h"
#include "gameplay.h"
#include <stdlib.h>

//
// GLOBALS
//

Entity npc = (Entity){0};

//
// LOCAL TYPES
//

typedef enum Npc_states
{
    THINKING,
    READY,
    IDLE
} Npc_state;

//
// LOCAL FUNCTIONS
//

void npc_update(float delta_time);
void check_npc_event(float delta_time);
float entity_get_y_center(Entity entity);
float npc_get_time_react();
float npc_ease(float percent, float x0, float x1);

//
// LOCAL VARIABLES
//

float npc_speed = 0;
float npc_time_react;
float npc_min_time_react;
float npc_max_time_react = 0;
float ball_prev_vel_x;
float ball_prev_vel_y;
Npc_state npc_state;
Vector2 npc_current_target;
float easing_duration;
float easing_elapsed_time;
float easing_x0;
float easing_x1;

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

    npc_speed = PADDLE_SPEED;

    npc_state = READY;
    npc_min_time_react = NPC_MIN_TIME_REACTION;
    npc_max_time_react = NPC_MAX_TIME_REACTION;
    npc_time_react = npc_get_time_react();
    ball_prev_vel_x = 0;
    ball_prev_vel_y = 0;
}

void npc_update(float delta_time)
{
    check_npc_event(delta_time);
    if (npc_state == THINKING)
    {
        // TODO;
    }

    if (npc_state == READY)
    {
        if (ball_destination.y != npc_current_target.y && ball.velocity.x > 0)
            npc_set_current_target();
    }

    if (npc_current_target.x != 0 && easing_elapsed_time < easing_duration && ball.velocity.x > 0)
    {

        easing_elapsed_time += delta_time;
        npc.bounds.y = npc_ease(easing_elapsed_time / easing_duration, easing_x0, easing_x1);
    }

    if (npc.bounds.y < 4)
    {
        npc.bounds.y = 4;
    }
    else if (npc.bounds.y > (SCREEN_HEIGHT - 4 - npc.bounds.height))
    {
        npc.bounds.y = SCREEN_HEIGHT - 4 - npc.bounds.height;
    }
}

void check_npc_event(float delta_time)
{

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

    // wall bounce
    if (ball_prev_vel_y * ball.velocity.y < 0)
    {
        npc_state = READY;
    }

    // player paddle bounce
    if (ball_prev_vel_x * ball.velocity.x < 0 && ball.velocity.x > 0)
    {
        npc_state = THINKING;
        npc_time_react = npc_get_time_react();
    }

    ball_prev_vel_x = ball.velocity.x;
    ball_prev_vel_y = ball.velocity.y;
}

// Easing function inOutCubic
float npc_ease(float percent, float x0, float x1)
{
    float p = percent;
    float factor = p < 0.5 ? 4 * p * p * p : 1 - (-2 * p + 2) * (-2 * p + 2) * (-2 * p + 2) / 2;
    return x0 + (x1 - x0) * factor;
}

void npc_set_current_target()
{
    npc_current_target.x = ball_destination.x;
    if (ball_destination.y < 4)
        ball_destination.y = 4 + npc.bounds.height / 2;
    if (ball_destination.y > (480 - 4))
        ball_destination.y = (480 - 4) - npc.bounds.height / 2;

    npc_current_target.y = ball_destination.y;
    easing_x0 = npc.bounds.y;
    easing_x1 = npc_current_target.y - npc.bounds.height / 2;
    easing_duration = abs(easing_x1 - easing_x0) / (npc_speed * 0.8);
    easing_elapsed_time = 0;
}

float entity_get_y_center(Entity entity)
{
    return entity.bounds.y + entity.bounds.height;
}

float npc_get_time_react()
{
    return (drand48() * (npc_max_time_react - npc_min_time_react) + npc_min_time_react);
}

void npc_reset()
{
    npc_state = THINKING;
    npc_time_react = npc_get_time_react();
}