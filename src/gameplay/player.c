#include "defs.h"
#include "gameplay.h"

Entity player = (Entity){0}; // global

int paddle_half = 0;

void player_draw()
{
    DrawTextureRec(texture_atlas, player.frame_rect, (Vector2){player.bounds.x, player.bounds.y}, WHITE);
}

void player_init()
{
    player.frame_rect = PADDLE_FRAME_RECT;
    float height = player.frame_rect.height;
    float width = player.frame_rect.width;
    paddle_half = height / 2;
    player.bounds = (Rectangle){PADDLE_H_MARGIN, SCREEN_HEIGHT / 2 - height / 2, width, height};
}

void player_update(float deltaTime)
{
    player.bounds.y = (player.bounds.y + GetMouseDelta().y);

    if (player.bounds.y < 4)
    {
        player.bounds.y = 4;
    }
    else if (player.bounds.y > (SCREEN_HEIGHT - 4 - player.bounds.height))
    {
        player.bounds.y = SCREEN_HEIGHT - 4 - player.bounds.height;
    }
}