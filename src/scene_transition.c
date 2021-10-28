#include "defs.h"
#include <math.h>

void init_scene(int scene);
void draw_scene(int scene);
void unload_scene(int scene);

int scene_source;
int scene_target;
float duration = TRANSITION_DURATION;
float time_elapsed;
int alpha;

void scene_transition_init(int source, int target)
{
    scene_source = source;
    scene_target = target;
    time_elapsed = 0;
    alpha = 0;
    init_scene(scene_target);
}

void scene_transition_update(float delta_time)
{
    time_elapsed += delta_time;
    if (time_elapsed > duration)
    {
        unload_scene(scene_source);
        current_scene = scene_target;
    }
    // y = ax^2 + bx + c ; max = 255
    float x = time_elapsed / duration;
    alpha = (int)(-1020*x*x + 1020*x);
}

void scene_transition_draw()
{

    if (time_elapsed < duration / 2)
    {
        draw_scene(scene_source);
    }
    else
    {
        draw_scene(scene_target);
    }
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){124, 63, 88, alpha});
}

void draw_scene(int scene)
{
    switch (scene)
    {
    case SCENE_GAMEPLAY:
        scene_gameplay_draw();
        break;
    case SCENE_TITLE:
        scene_title_draw();
        break;
    case SCENE_GAMEOVER:
        scene_gameover_draw();
        break;

    default:
        break;
    }
}

void init_scene(int scene)
{
    switch (scene)
    {
    case SCENE_GAMEPLAY:
        scene_gameplay_init();
        break;
    case SCENE_TITLE:
        scene_title_init();
        break;
    case SCENE_GAMEOVER:
        scene_gameover_init();
        break;

    default:
        break;
    }
}

void unload_scene(int scene)
{
    switch (scene)
    {
    case SCENE_GAMEPLAY:
        scene_gameplay_destroy();
        break;
    case SCENE_TITLE:
        scene_title_destroy();
        break;
    case SCENE_GAMEOVER:
        scene_gameover_destroy();
        break;

    default:
        break;
    }
}

void scene_transition_destroy()
{
    // NOTHING TO UNLOAD
}