#include "defs.h"
#include "gameplay.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

void run_frame();
void unload_current_scene();

enum Scene current_scene = SCENE_TITLE;
Texture2D texture_atlas;
Font font;

int main()
{
    // Initialization
    //---------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);

    SetWindowState(FLAG_VSYNC_HINT);

    InitAudioDevice();

    font = LoadFontEx("assets/font_silkscreen/slkscr.ttf", 36, 0, 98);
    texture_atlas = LoadTexture("assets/atlas640x640_pong.png");

    scene_title_init();

#if defined(PLATFORM_WEB)
    // https://emscripten.org/docs/api_reference/emscripten.h.html#c.emscripten_set_main_loop
    emscripten_set_main_loop(run_frame, -1, 1);
#else
    while (!WindowShouldClose())
    {
        run_frame();
    }
#endif

    // Unload resources
    unload_current_scene();
    UnloadTexture(texture_atlas);
    UnloadFont(font);
    CloseAudioDevice(); // Close audio context
    CloseWindow();      // Close window and OpenGL context

    return 0;
}

void run_frame()
{
    switch (current_scene)
    {
    case SCENE_GAMEPLAY:
        scene_gameplay_update(GetFrameTime());
        BeginDrawing();
        ClearBackground(BLACK);
        scene_gameplay_draw();
        break;
    case SCENE_TITLE:
        scene_title_update();
        BeginDrawing();
        ClearBackground(BLACK);
        scene_title_draw();
        break;
    case SCENE_GAMEOVER:
        scene_gameover_update();
        BeginDrawing();
        ClearBackground(BLACK);
        scene_gameover_draw();
        break;
    case SCENE_TRANSITION:
        scene_transition_update(GetFrameTime());
        BeginDrawing();
        ClearBackground(BLACK);
        scene_transition_draw();
        break;

    default:
        break;
    }

    EndDrawing();
}

void unload_current_scene()
{
    switch (current_scene)
    {
    case SCENE_GAMEPLAY:
        scene_gameplay_destroy();
        break;
    case SCENE_GAMEOVER:
        scene_gameover_destroy();
        break;
    case SCENE_TITLE:
        scene_title_destroy();
        break;
    default:
        break;
    }
}