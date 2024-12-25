#include <stdio.h>
#include <uchar.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

#include "canvas.h"

//Colors Foreground = {
//    .black = 30,
//    .red = 31,
//    .green = 32,
//    .yellow = 33,
//    .blue = 34,
//    .magenta = 35,
//    .cyan = 36,
//    .white = 37,
//
//    .bblack = 90,
//    .bred = 91,
//    .bgreen = 92,
//    .byellow = 93,
//    .bblue = 94,
//    .bmagenta = 95,
//    .bcyan = 96,
//    .bwhite = 97,
//};
//
//Colors Background = {
//
//    .black = 40,
//    .red = 41,
//    .green = 42,
//    .yellow = 43,
//    .blue = 44,
//    .magenta = 45,
//    .cyan = 46,
//    .white = 47,
//
//    .bblack = 100,
//    .bred = 101,
//    .bgreen = 102,
//    .byellow = 103,
//    .bblue = 104,
//    .bmagenta = 105,
//    .bcyan = 106,
//    .bwhite = 107,
//};

Colors Color = {

    .black = 0,
    .white = 15,

    .red = 9,
    .orange = 215,
    .yellow = 226,
    .blue = 21,
    .cyan = 14,

    .coral = 221,
    .pink = 219,

};

void moveCursor(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { x + 1, y + 1 };
    SetConsoleCursorPosition(hConsole, pos);
}

Canvas* createCanvas(int w, int h) {
    system("cls");
    Canvas* canvas = (Canvas*) malloc(sizeof(Canvas));
    int size = w * h;
    char16_t* buffer1 = (char16_t*)malloc(size * sizeof(char16_t));
    char16_t* buffer2 = (char16_t*)malloc(size * sizeof(char16_t));
    char* background = (char*)malloc(size * sizeof(char) * 9);
    char* foreground = (char*)malloc(size * sizeof(char) * 9);
    char* updated = (char*)malloc(size * sizeof(char));
    if (!buffer1) return NULL;
    if (!buffer2) return NULL;
    if (!updated) return NULL;
    if (!foreground) return NULL;
    if (!background) return NULL;
    //printf(u"\x1b[47m");
    printf("\u250F\u2501");
    for (int x = 1; x < w; x++) printf("\u2501\u2501");
    printf("\u2501\u2513\n\x1b[0m");
    for (int y = 0; y < h; y++) {
        printf("\u2503");
        for (int x = 0; x < w; x++) {
            int i = (x + y * w);
            buffer1[i] = u' ';
            buffer2[i] = u' ';
            updated[i] = 1;
            background[i * 9] = Color.black;
            foreground[i * 9] = Color.white;
            printf("\x1b[0m%lc%lc", u' ', u' ');
        }
        printf("\u2503%lc", u'\n');
    }
    printf("\u2517\u2501");
    for (int x = 1; x < w; x++) printf("\u2501\u2501");
    printf("\u2501\u251B\n");
    canvas->w = w;
    canvas->h = h;
    canvas->background = background;
    canvas->foreground = foreground;
    canvas->buffer1 = buffer1;
    canvas->buffer2 = buffer2;
    canvas->updated = updated;
    return canvas;
}


void flushCanvas(Canvas* canvas) {
    for (int y = 0; y < canvas->h; y++) {
        for (int x = 0; x < canvas->w; x++) {
            if (!canvas->updated[x + y * ((canvas->w))]) continue;
            moveCursor(x * 2, y);
            //printf("\x1b[0;%dm", canvas->foreground[x + y * ((canvas->w))]);
            //printf("\x1b[0;%dm", canvas->background[x + y * ((canvas->w))]);
            //printf("\x1b[0;%dm\x1b[0;%dm", canvas->foreground[x + y * ((canvas->w))], canvas->background[x + y * ((canvas->w))]);
            //printf("\x1b[0;%dm\x1b[%dm", canvas->foreground[x + y * ((canvas->w))], canvas->background[x + y * ((canvas->w))]);
            int pack = x + y * ((canvas->w));

            //printf("\x1b[0;%dm\x1b[%dm", canvas->foreground[pack], canvas->background[pack]);


            printf("\033[38;5;%dm", canvas->foreground[pack]);
            printf("\033[48;5;%dm", canvas->background[pack]);

            if (canvas->buffer1[pack] == u'━') printf("\033[38;5;%dm", Color.pink);

            printf("%lc", canvas->buffer1[pack]);

            if (canvas->buffer2[pack] == u'━') printf("\033[38;5;%dm", Color.pink);

            printf("%lc", canvas->buffer2[pack]);
            canvas->updated[x + y * ((canvas->w))] = 0;
        }
    }
    moveCursor(-1, -1);
}

void setBackground(Canvas* canvas, int color, int x, int y) {
    if (x < 0 || x >= canvas->w) return;
    if (y < 0 || y >= canvas->h) return;
    if (canvas->background[x + y * canvas->w] != color) canvas->updated[x + y * canvas->w] = 1;
    canvas->background[x + y * canvas->w] = color;
}

void setForeground(Canvas* canvas, int color, int x, int y) {
    if (x < 0 || x >= canvas->w) return;
    if (y < 0 || y >= canvas->h) return;
    if (canvas->foreground[x + y * canvas->w] != color) canvas->updated[x + y * canvas->w] = 1;
    canvas->foreground[x + y * canvas->w] = color;
}

void setChar(Canvas* canvas, char16_t c, int x, int y) {
    if (x < 0 || x >= canvas->w) return;
    if (y < 0 || y >= canvas->h) return;
    canvas->buffer1[x + y * canvas->w] = c;
    canvas->buffer2[x + y * canvas->w] = c;
    canvas->updated[x + y * canvas->w] = 1;
}

void setChars(Canvas* canvas, char16_t c1, char16_t c2, int x, int y) {
    if (x < 0 || x >= canvas->w) return;
    if (y < 0 || y >= canvas->h) return;
    canvas->buffer1[x + y * canvas->w] = c1;
    canvas->buffer2[x + y * canvas->w] = c2;
    canvas->updated[x + y * canvas->w] = 1;
}