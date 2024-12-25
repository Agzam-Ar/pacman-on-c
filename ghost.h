#pragma once

#include "canvas.h"
#include "map.h"
#include "player.h"

typedef struct Ghost {

    int x;
    int y;
    int lx;
    int ly;
    void (*getTarget)(Player*, Map*, int* x, int* y);
    int color;

    char atHome;

} Ghost;


Ghost blinky, pinky, inky, clyde;

void loadGhosts();
void updateGhost(Ghost* ghost, Player* player, Canvas* canvas, Map* map);
