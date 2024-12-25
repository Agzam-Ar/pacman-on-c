#pragma once

#include "canvas.h"
#include "map.h"

typedef struct Player {

    int x, y;
    int vx, vy;

    int hp;

} Player;


void updatePlayer(Player* player, Canvas* canvas, Map* map);