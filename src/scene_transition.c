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

// [sc1init,sc1draw,sc1destroy, ..., sc12init,sc12draw,sc12destroy]
void (*scene_func[9])() = {
    scene_title_init,
    scene_title_draw,
    scene_title_destroy,
    scene_gameplay_init,
    scene_gameplay_draw,
    scene_gameplay_destroy,
    scene_gameover_init,
    scene_gameover_draw,
    scene_gameover_destroy};

void scene_transition_init(int source, int target)
{
    current_scene = SCENE_TRANSITION;
    scene_source = source;
    scene_target = target;
    time_elapsed = 0;
    alpha = 0;
    (*scene_func[scene_target * 3 + 0])();
}

void scene_transition_update(float delta_time)
{
    time_elapsed += delta_time;
    if (time_elapsed > duration)
    {
        (*scene_func[scene_source * 3 + 2])();
        current_scene = scene_target;
    }
    // y = ax^2 + bx + c ; max = 255
    float x = time_elapsed / duration;
    alpha = (int)(-1020 * x * x + 1020 * x);
}

void scene_transition_draw()
{

    if (time_elapsed < duration / 2)
    {
        (*scene_func[scene_source * 3 + 1])();
    }
    else
    {
        (*scene_func[scene_target * 3 + 1])();
    }
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){124, 63, 88, alpha});
}

void scene_transition_destroy()
{
    // NOTHING TO UNLOAD
}