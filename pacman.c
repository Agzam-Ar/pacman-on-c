// pacman.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include <windows.h>
#include <stdio.h>
#include <locale.h>
#include "canvas.h"
#include "map.h"
#include "input.h"
#include "player.h"
#include "ghost.h"

#define width  28
#define height 36

void moveCursor(int x, int y);
void setChar(Canvas* canvas, char16_t c, int x, int y);
void setChars(Canvas* canvas, char16_t c1, char16_t c2, int x, int y);
void setBackground(Canvas* canvas, int color, int x, int y);
void setForeground(Canvas* canvas, int color, int x, int y);
Canvas* createCanvas(int w, int h);
void flushCanvas(Canvas* canvas);


void drawMap(Canvas* canvas, int w, int h, int color, char full);
Tile* getTile(Map* map, int x, int y);
Map* createMap(int w, int h);
void drawTile(Canvas* canvas, Map* map, int x, int y);

void setupKeyboard();
void handleKeys();

void updatePlayer(Player* player, Canvas* canvas, Map* map);

void loadGhosts();
void updateGhost(Ghost* ghost, Player* player, Canvas* canvas, Map* map);

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

    drawMap(canvas, width, height, Color.blue, 1);
    for (int y = 0; y < 3; y++) {
         for (int x = 0; x < width; x++) {
             setForeground(canvas, Color.white, x, y);
         }
     }
    loadGhosts();

    Player player = {
        .x = sw / 2,
        .y = sh * 3 / 4 - 1,
        .vx = 0,
        .vy = 0,
        .hp = 3,
    };

    Map* map = createMap(width, height);
    if (!map) return -1;

    int ticksBeforeSec = 0;
    int time = 0;

    map->time = 0;
    map->combo = 0;

    int ghostReloadMs = 0;
    int ghostSpeedMs = 1500 / tps;

    while (map->food > 0 && Input.exit <= 0) {
        adjustWindowSize();

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                setBackground(canvas, Color.black, x, y);
            }
        }

        updatePlayer(&player, canvas, map);
        ghostReloadMs += 1000 / tps;

        if (ghostReloadMs > ghostSpeedMs) {
            ghostReloadMs -= ghostSpeedMs;
            updateGhost(&blinky, &player, canvas, map);
            updateGhost(&pinky, &player, canvas, map);
            updateGhost(&inky, &player, canvas, map);
            updateGhost(&clyde, &player, canvas, map);
        }
        int score = map->score;
        for (int x = 0; score > 0; x++) {
            setChars(canvas, u'0' + score % 10, u' ', 6 - x, 1);
            score /= 10;
        }

        if (Input.debug) {
            score = map->time;
            for (int x = 0; score > 0; x++) {
                setChars(canvas, u'0' + score % 10, u' ', 6 - x, 2);
                score /= 10;
            }

            score = map->bonus;
            for (int x = 0; score > 0; x++) {
                setChars(canvas, u'0' + score % 10, u' ', 12 - x, 2);
                score /= 10;
            }
        }

        for (int i = 0; i < 4; i++) {
            Ghost* ghost = ghosts[i];
            if (!ghost->eyeMode && ghost->x == player.x && ghost->y == player.y) {
                if (map->bonus > 0) {
                    ghost->eyeMode = 1;
                    map->combo++;
                    map->score += map->combo * 1000;
                }
            }
        }


        flushCanvas(canvas);
        Sleep(1000 / tps);
        if (map->bonus > 0) {
            map->bonus -= 1000 / tps;
        } else if (map->bonus < 0) {
            map->bonus = 0;
            map->combo = 0;
        }

        ticksBeforeSec++;
        if (ticksBeforeSec == tps) {
            ticksBeforeSec = 0;
            time++;
            map->time = time;
        }
    }
    if (Input.exit <= 0) {
       for (int i = 0; i < 8; i++) {
            drawMap(canvas, width, height, i%2 ? Color.white : Color.blue, 0);
            int score = map->score;
            for (int x = 0; score > 0; x++) {
                setChars(canvas, u'0' + score % 10, u' ', 6 - x, 1);
                //setChars(canvas, u'0' + score % 10, u' ', map->w / 2 + 2 - x, map->h/2-1);
                score /= 10;
            }
            flushCanvas(canvas);
            Sleep(10);
        }
    }
    system("cls");
    return 0;
}