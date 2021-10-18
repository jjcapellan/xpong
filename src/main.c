#include "defs.h"


#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

void run_frame();
enum Scene current_scene = SCENE_TITLE;
Texture2D texture_atlas;

int main()
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);

    InitAudioDevice();

    texture_atlas = LoadTexture("assets/atlas640x640_pong.png");

    scene_title_init();



#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(run_frame, 60, 1);
#else
    while (!WindowShouldClose())
    {
        run_frame();
    }
#endif

    // Unload resources

    CloseAudioDevice(); // Close audio context
    CloseWindow(); // Close window and OpenGL context

    return 0;
}

void run_frame()
{
    switch (current_scene)
    {
    case SCENE_GAMEPLAY:
        scene_gameplay_update(GetFrameTime());
        scene_gameplay_draw();
        break;
    case SCENE_TITLE:
        scene_title_update();
        scene_title_draw();
        break;
    case SCENE_GAMEOVER:
        /* code */
        break;
    
    default:
        break;
    }
}