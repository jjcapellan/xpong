#ifndef DEFS_H
#define DEFS_H
#include "raylib.h"

#define SCREEN_TITLE "Game Title" // window title
#define SCREEN_WIDTH 640          // px
#define SCREEN_HEIGHT 480         // px

#define PADDLE_H_MARGIN 12 // px, horizontal margin to screen border
#define WORLD_BOUNDS \
    (Rectangle) { 0, 4, 640, 480 - 8 } // {px,px,px,px}
#define PADDLE_SPEED 200               // px/sec
#define BALL_SPEED 100                 // px/sec

enum Scene
{
    SCENE_TITLE = 0,
    SCENE_GAMEPLAY,
    SCENE_GAMEOVER
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

#endif