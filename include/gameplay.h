#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "raylib.h"

#define PADDLE_FRAME_RECT \
    (Rectangle) { 0, 480, 12, 72 } // {px,px,px,px} Rectangle frame on sprite sheet
#define PADDLE_H_MARGIN 12         // px, horizontal margin to screen border
#define PADDLE_MAX_ANGLE 40        // degrees, angle added to ball velocity vector when collide paddle edges
#define PADDLE_SPEED 260           // px/sec
#define NPC_MIN_SPEED_FACTOR 0.1   // float, min multiplier applied to normal speed
#define NPC_MAX_SPEED_FACTOR 1.1   // float, max multiplier applied to normal speed
#define NPC_MIN_TIME_REACTION 0.2  // seconds

typedef struct Entity
{
    Rectangle frame_rect;
    Rectangle bounds;
    Vector2 velocity; // px/sec
} Entity;

void player_init();
void player_update(float delta_time);

void npc_init();
void npc_update(float delta_time);
void npc_reset();

extern Entity player;
extern Entity npc;
extern Entity ball;

#endif
