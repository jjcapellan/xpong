#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "raylib.h"

//
// CONSTANTS
//

#define FIELD_FRAME_RECT \
    (Rectangle) { 0, 0, 640, 480 } // {px,px,px,px} Rectangle frame on sprite sheet

#define PADDLE_FRAME_RECT \
    (Rectangle) { 0, 480, 12, 72 }      // {px,px,px,px} Rectangle frame on sprite sheet
#define PADDLE_H_MARGIN 12              // px, horizontal margin to screen border
#define PADDLE_MAX_ANGLE 40             // degrees, angle added to ball velocity vector when collide paddle edges
#define NPC_ERRORS_LEVEL_1 0.40         // max npc probability to fail
#define NPC_ERRORS_LEVEL_2 0.35         // max npc probability to fail
#define NPC_ERRORS_LEVEL_3 0.30         // max npc probability to fail
#define NPC_ERRORS_LEVEL_4 0.25         // max npc probability to fail
#define NPC_ERRORS_LEVEL_5 0.20         // max npc probability to fail
#define NPC_ERRORS_LEVEL_6 0.15         // max npc probability to fail
#define NPC_ERRORS_LEVEL_7 0.10         // max npc probability to fail
#define NPC_ERRORS_LEVEL_8 0.05         // max npc probability to fail
#define NPC_TIMING_ERROR_MULTIPLIER 1.9 // makes npc to arrive too late to the ball position
#define NPC_ERRORS_DISTANCE_BONUS 0.75  // float, min distance between ball destination and npc which raises probability of fail, expressed as a percentage of the field width.
#define NPC_ERRORS_BONUS 0.3            // float, this value is added to the probability of fail when gap between ball destination and npc is greater than bonus distance.
#define NPC_REACTION_TIME 0.1           // seconds

#define BALL_FRAME_RECT \
    (Rectangle) { 12, 480, 12, 12 } // {px,px,px,px} Rectangle frame on sprite sheet
#define BALL_SPEED_LEVEL_1 480      // px/sec
#define BALL_SPEED_LEVEL_2 560      // px/sec
#define BALL_SPEED_LEVEL_3 640      // px/sec
#define BALL_SPEED_LEVEL_4 720      // px/sec
#define BALL_SPEED_LEVEL_5 780      // px/sec
#define BALL_SPEED_LEVEL_6 840      // px/sec
#define BALL_SPEED_LEVEL_7 920      // px/sec
#define BALL_SPEED_LEVEL_8 980      // px/sec
#define BALL_SPEED_START 190        // px/sec
#define BALL_MAX_ANGLE 65           // degrees
#define WORLD_BOUNDS \
    (Rectangle) { 0, 4, 640, 480 - 8 } // {px,px,px,px}

#define PARTICLE_FRAME_RECT \
    (Rectangle) { 0, 480, 6, 6 }  // {px,px,px,px} Rectangle frame on sprite sheet
#define PARTICLE_MIN_SCALE 0.3    // float, min scale based on ball size
#define PARTICLE_MAX_SCALE 0.7    // float, max scale based on ball size
#define PARTICLE_MAX_DURATION 0.5 // seconds
#define PARTICLE_MIN_DURATION 0.2 // seconds
#define PARTICLES_MAX_NUMBER 6    // int

#define SCORE_TEXT_COLOR \
    (Color) { 249, 168, 117, 80 } // Color
#define LEVEL_TEXT_COLOR \
    (Color) { 249, 168, 117, 255 } // Color
#define SCORE_TEXT_SIZE 144        // px

#ifndef DEBUG
#define POINTS_PER_LEVEL 3 // int
#else
#define POINTS_PER_LEVEL 1000 // int
#endif

//
// TYPE DEFINITIONS
//
typedef struct Entity
{
    Rectangle frame_rect;
    Rectangle bounds;
    Vector2 velocity; // px/sec
} Entity;

typedef struct Particle
{
    Rectangle frame_rect;
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float scale;
    float time;
    bool alive;
} Particle;

typedef struct Level
{
    float npc_max_error; // Max probability to fail (0 - 1).
    float ball_speed;
} Level;

//
// FUNCTIONS
//

void player_init();
void player_update(float delta_time);
void player_draw();

void npc_init();
void npc_update(float delta_time);
void npc_set_current_target();
void npc_reset();
void npc_draw();

void ball_init();
void ball_update(float delta_time);
void ball_reset(bool is_player);
void ball_draw();

void particles_init();
void particles_update(float delta_time);
void particles_reset(Vector2 position);

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
extern Particle particles[];
extern float npc_max_error;
extern float ball_speed;
#ifdef DEBUG
extern int touches;
extern int points;
extern int errors;
#endif

#endif
