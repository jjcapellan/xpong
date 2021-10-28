#ifndef DEFS_H
#define DEFS_H
#include "raylib.h"

#define SCREEN_TITLE "Game Title" // window title
#define SCREEN_WIDTH 640          // px
#define SCREEN_HEIGHT 480         // px

#define TRANSITION_DURATION 0.8 // secs, transition duration between scenes

#define WORLD_BOUNDS \
    (Rectangle) { 0, 4, 640, 480 - 8 } // {px,px,px,px}

#define PADDLE_FRAME_RECT \
    (Rectangle) { 0, 480, 12, 72 } // {px,px,px,px} Rectangle frame on sprite sheet
#define PADDLE_H_MARGIN 12         // px, horizontal margin to screen border
#define PADDLE_MAX_ANGLE 40        // degrees, angle added to ball velocity vector when collide paddle edges
#define PADDLE_SPEED 260           // px/sec
#define NPC_MIN_SPEED_FACTOR 0.1   // float, min multiplier applied to normal speed
#define NPC_MAX_SPEED_FACTOR 1.1   // float, max multiplier applied to normal speed

#define BALL_FRAME_RECT \
    (Rectangle) { 12, 480, 12, 12 } // {px,px,px,px} Rectangle frame on sprite sheet
#define BALL_SPEED 380              // px/sec
#define BALL_MAX_ANGLE 65           // degrees

#define SCORE_TEXT_COLOR \
    (Color) { 249, 168, 117, 80 } // Color
#define SCORE_TEXT_SIZE 144       // px

enum Scene
{
    SCENE_TITLE = 0,
    SCENE_GAMEPLAY,
    SCENE_GAMEOVER,
    SCENE_TRANSITION
};

extern enum Scene current_scene;
extern Texture2D texture_atlas;

void scene_title_init();
void scene_title_update();
void scene_title_draw();
void scene_title_destroy();

void scene_gameplay_init();
void scene_gameplay_update(float deltaTime);
void scene_gameplay_draw();
void scene_gameplay_destroy();

void scene_gameover_init();
void scene_gameover_update();
void scene_gameover_draw();
void scene_gameover_destroy();

void scene_transition_init();
void scene_transition_update(float deltaTime);
void scene_transition_draw();

#endif