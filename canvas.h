#pragma once
#include <uchar.h>

typedef struct Canvas {

    int w, h;

    char16_t* buffer1;
    char16_t* buffer2;
    char* background;
    char* foreground;
    char* updated;

} Canvas;

typedef struct Colors {
    char black;
    char red;
    char green;
    char yellow;
    char blue;
    char magenta;
    char cyan;
    char white;
    char bblack;
    char bred;
    char bgreen;
    char byellow;
    char bblue;
    char bmagenta;
    char bcyan;
    char bwhite;
} Colors;

Colors Foreground, Background;
void moveCursor(int x, int y);
void setChar(Canvas* canvas, char16_t c, int x, int y);
void setChars(Canvas* canvas, char16_t c1, char16_t c2, int x, int y);
void setBackground(Canvas* canvas, int color, int x, int y);
void setForeground(Canvas* canvas, int color, int x, int y);
Canvas* createCanvas(int w, int h);
void flushCanvas(Canvas* canvas);
