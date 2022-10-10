#ifndef DEFS_H
#define DEFS_H
#include "raylib.h"

#define SCREEN_TITLE "XPONG" // window title
#define SCREEN_WIDTH 640     // px
#define SCREEN_HEIGHT 480    // px

#define TRANSITION_DURATION 0.8 // secs, transition duration between scenes

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

void scene_transition_init(int source, int target);
void scene_transition_update(float deltaTime);
void scene_transition_draw();

// shared variables
extern int player_total_score;
extern int player_max_level;
extern int npc_total_score;

// shared resources
extern Font font;

#endif