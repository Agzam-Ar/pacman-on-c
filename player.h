#pragma once

#include "player.h"
#include "canvas.h"
#include "map.h"

typedef struct Player {

    int x, y;
    int vx, vy;

} Player;


void updatePlayer(Player* player, Canvas* canvas, Map* map);