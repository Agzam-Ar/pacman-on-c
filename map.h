#pragma once
#include "canvas.h"

typedef struct Tile {

    char food;
    char solid;

} Tile;


typedef struct Map {

    int w, h;
    Tile* tiles;

    short food;
    short maxFood;
    char playing;
    short wave;
    int time;

    int score;

    int bonus;
    int combo;

} Map;


void drawMap(Canvas* canvas, int w, int h, int color, char full);
Tile* getTile(Map* map, int x, int y);
Map* createMap(int w, int h);
void drawTile(Canvas* canvas, Map* map, int x, int y);