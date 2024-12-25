#pragma once
#include <uchar.h>

typedef struct Canvas {

    int w, h;

    char16_t* buffer1;
    char16_t* buffer2;
    int* background;
    int* foreground;
    char* updated;

} Canvas;

typedef struct Colors {
    int black;
    int red;
    int green;
    int yellow;
    int blue;
    int magenta;
    int cyan;
    int white;
    int bblack;
    int bred;
    int bgreen;
    int byellow;
    int bblue;
    int bmagenta;
    int bcyan;
    int bwhite;

    int orange;
    int coral;
    int pink;

} Colors;

Colors Color; // Foreground, Background, 
void moveCursor(int x, int y);
void setChar(Canvas* canvas, char16_t c, int x, int y);
void setChars(Canvas* canvas, char16_t c1, char16_t c2, int x, int y);
void setBackground(Canvas* canvas, int color, int x, int y);
void setForeground(Canvas* canvas, int color, int x, int y);
Canvas* createCanvas(int w, int h);
void flushCanvas(Canvas* canvas);
