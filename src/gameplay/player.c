#include "defs.h"
#include "gameplay.h"

Entity player = (Entity){0}; // global

void player_init(){
    player.frame_rect = PADDLE_FRAME_RECT;
    float height = player.frame_rect.height;
    float width = player.frame_rect.width;
    player.bounds = (Rectangle){PADDLE_H_MARGIN, SCREEN_HEIGHT / 2 - height / 2, width, height};
    player.velocity = (Vector2){0, 0};
}

void player_update(float deltaTime)
{
    player.bounds.y += player.velocity.y * deltaTime;

    if (player.bounds.y < 4)
    {
        player.bounds.y = 4;
    }
    else if (player.bounds.y > (SCREEN_HEIGHT - 4 - player.bounds.height))
    {
        player.bounds.y = SCREEN_HEIGHT - 4 - player.bounds.height;
    }

    player.velocity.y = 0;
}