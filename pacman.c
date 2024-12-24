// pacman.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <windows.h>
#include <stdio.h>
#include <locale.h>
#include "canvas.h"
#include "map.h"
#include "input.h"
#include "player.h"

#define width  28
#define height 36

void moveCursor(int x, int y);
void setChar(Canvas* canvas, char16_t c, int x, int y);
void setChars(Canvas* canvas, char16_t c1, char16_t c2, int x, int y);
void setBackground(Canvas* canvas, int color, int x, int y);
void setForeground(Canvas* canvas, int color, int x, int y);
Canvas* createCanvas(int w, int h);
void flushCanvas(Canvas* canvas);


void drawMap(Canvas* canvas, int w, int h, int color);
Tile* getTile(Map* map, int x, int y);
Map* createMap(int w, int h);

void setupKeyboard();
void handleKeys();

void updatePlayer(Player* player, Canvas* canvas, Map* map);

struct SMALL_RECT {
    SHORT Left;
    SHORT Top;
    SHORT Right;
    SHORT Bottom;
};

void adjustWindowSize() {
    HANDLE wHnd;    // Handle to write to the console.
    HANDLE rHnd;    // Handle to read from the console.
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    rHnd = GetStdHandle(STD_INPUT_HANDLE);
    SMALL_RECT windowSize = { 0, 0, width * 2 + 2, height + 1 };
    SetConsoleWindowInfo(wHnd, 1, &windowSize);
    COORD bufferSize = { width * 2 + 2, height + 2};
    SetConsoleScreenBufferSize(wHnd, bufferSize);
}

int main() {
    int sw = width;
    int sh = height;
    Canvas* canvas = createCanvas(sw, sh);
    if (!canvas) return -1;
    setlocale(LC_ALL, "");

    int tps = 15;

    setupKeyboard();

    drawMap(canvas, width, height, Foreground.blue);

    Player player = {
        .x = sw / 2,
        .y = sh * 3 / 4 - 1,
        .vx = 0,
        .vy = 0,
    };

    Map* map = createMap(width, height);
    if (!map) return -1;

    map->food = 10;

    while (map->food > 0) {
        adjustWindowSize();
        updatePlayer(&player, canvas, map);
        flushCanvas(canvas);
        Sleep(1000 / tps);
    }

    for (int i = 0; i < 8; i++) {
        drawMap(canvas, width, height, i%2 ? Foreground.white : Foreground.blue);
        flushCanvas(canvas);
        Sleep(10);
    }
    return 0;
}