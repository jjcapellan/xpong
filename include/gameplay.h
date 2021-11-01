#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "raylib.h"


//
// CONSTANTS
//

#define FIELD_FRAME_RECT \
    (Rectangle) { 0, 0, 640, 480 } // {px,px,px,px} Rectangle frame on sprite sheet

#define PADDLE_FRAME_RECT \
    (Rectangle) { 0, 480, 12, 72 } // {px,px,px,px} Rectangle frame on sprite sheet
#define PADDLE_H_MARGIN 12         // px, horizontal margin to screen border
#define PADDLE_MAX_ANGLE 40        // degrees, angle added to ball velocity vector when collide paddle edges
#define PADDLE_SPEED 280           // px/sec
#define NPC_MIN_TIME_REACTION 0.0  // seconds
#define NPC_MAX_TIME_REACTION 0.3  // seconds

#define BALL_FRAME_RECT \
    (Rectangle) { 12, 480, 12, 12 } // {px,px,px,px} Rectangle frame on sprite sheet
#define BALL_SPEED 480              // px/sec
#define BALL_SPEED_START 190        // px/sec
#define BALL_MAX_ANGLE 65           // degrees
#define WORLD_BOUNDS \
    (Rectangle) { 0, 4, 640, 480 - 8 } // {px,px,px,px}

#define SCORE_TEXT_COLOR \
    (Color) { 249, 168, 117, 80 } // Color
#define LEVEL_TEXT_COLOR \
    (Color) { 249, 168, 117, 255 } // Color
#define SCORE_TEXT_SIZE 144        // px

#define POINTS_PER_LEVEL 3 // int


//
// TYPE DEFINITIONS
//
typedef struct Entity
{
    Rectangle frame_rect;
    Rectangle bounds;
    Vector2 velocity; // px/sec
} Entity;


//
// FUNCTIONS
//

void player_init();
void player_update(float delta_time);

void npc_init();
void npc_update(float delta_time);
void npc_set_current_target();
void npc_reset();

void ball_init();
void ball_update(float delta_time);
void ball_reset(bool is_player);

void event_wall();
void event_paddle_bounce();
void event_npc_score();
void event_player_score();


//
// GLOBALS
//
extern Entity player;
extern Entity npc;
extern Entity ball;
extern Vector2 ball_destination;

#endif
