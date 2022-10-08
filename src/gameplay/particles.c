#include "defs.h"
#include "gameplay.h"
#include <stdlib.h>

Particle particles[PARTICLES_MAX_NUMBER];

void particles_init()
{
    for (int i = 0; i < PARTICLES_MAX_NUMBER; i++)
    {
        Particle *p = &particles[i];
        p->frame_rect = PARTICLE_FRAME_RECT;
        p->velocity = (Vector2){0, 0};
        p->position = (Vector2){0, 0};
        p->rotation = 0.0;
        p->scale = 1.0;
        p->time = 0.0;
        p->alive = false;
    }
}

void particles_reset(Vector2 position)
{
    for (int i = 0; i < PARTICLES_MAX_NUMBER; i++)
    {
        Particle *p = &particles[i];
        p->frame_rect = PARTICLE_FRAME_RECT;
        p->velocity = (Vector2){(float)(GetRandomValue(1, 200) - 100), (float)(GetRandomValue(1, 200) - 100)};
        p->position = (Vector2){position.x, position.y};
        p->rotation = (float)GetRandomValue(1, 360);
        p->scale = PARTICLE_MIN_SCALE + drand48() * (PARTICLE_MAX_SCALE - PARTICLE_MIN_SCALE);
        p->time = PARTICLE_MIN_DURATION + drand48() * (PARTICLE_MAX_DURATION - PARTICLE_MIN_DURATION);
        p->alive = true;
    }
}

void particles_update(float delta_time)
{
    for (int i = 0; i < PARTICLES_MAX_NUMBER; i++)
    {
        Particle *p = &particles[i];
        if (p->alive == true)
        {
            p->position = (Vector2){p->position.x + p->velocity.x * delta_time, p->position.y + p->velocity.y * delta_time};
            p->rotation += 0.01;
            p->time -= delta_time;
            if (p->time < 0)
            {
                p->alive = false;
            }
        }
    }
}