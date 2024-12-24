#pragma once
#include "canvas.h"

typedef struct Tile {

    char food;
    char solid;

} Tile;


typedef struct Map {

    int w, h;
    Tile* tiles;

    char food;
    char playing;

} Map;


void drawMap(Canvas* canvas, int w, int h, int color);
Tile* getTile(Map* map, int x, int y);
Map* createMap(int w, int h);