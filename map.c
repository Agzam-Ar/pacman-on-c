#include "map.h"
#include "canvas.h"
#include <uchar.h>
#include <stdlib.h>
#include <stdio.h>

char16_t* mapPreset = {
u"                  H I G H   S C O R E                   "
u"          0 0                                           "
u"                                                        "
u" ╭─────────────────────────╮ ╭─────────────────────────╮"
u" │ . . . . . . . . . . . . │ │ . . . . . . . . . . . . │"
u" │ . ╭─────╮ . ╭───────╮ . │ │ . ╭───────╮ . ╭─────╮ . │"
u" │ O │     │ . │       │ . │ │ . │       │ . │     │ O │"
u" │ . ╰─────╯ . ╰───────╯ . ╰─╯ . ╰───────╯ . ╰─────╯ . │"
u" │ . . . . . . . . . . . . . . . . . . . . . . . . . . │"
u" │ . ╭─────╮ . ╭─╮ . ╭─────────────╮ . ╭─╮ . ╭─────╮ . │"
u" │ . ╰─────╯ . │ │ . ╰─────╮ ╭─────╯ . │ │ . ╰─────╯ . │"
u" │ . . . . . . │ │ . . . . │ │ . . . . │ │ . . . . . . │"
u" ╰─────────╮ . │ ╰─────╮   │ │   ╭─────╯ │ . ╭─────────╯"
u"           │ . │ ╭─────╯   ╰─╯   ╰─────╮ │ . │          "
u"           │ . │ │         (B)         │ │ . │          "
u"           │ . │ │   ╔═════━━━═════╗   │ │ . │          "
u" ──────────╯ . ╰─╯   ║             ║   ╰─╯ . ╰──────────"
u"             .       ║ (I) (P) (C) ║       .            "
u" ──────────╮ . ╭─╮   ║             ║   ╭─╮ . ╭──────────"
u"           │ . │ │   ╚═════════════╝   │ │ . │          "
u"           │ . │ │                     │ │ . │          "
u"           │ . │ │   ╭─────────────╮   │ │ . │          "
u" ╭─────────╯ . ╰─╯   ╰─────╮ ╭─────╯   ╰─╯ . ╰─────────╮"
u" │ . . . . . . . . . . . . │ │ . . . . . . . . . . . . │"
u" │ . ╭─────╮ . ╭───────╮ . │ │ . ╭───────╮ . ╭─────╮ . │"
u" │ . ╰───╮ │ . ╰───────╯ . ╰─╯ . ╰───────╯ . │ ╭───╯ . │"
u" │ O . . │ │ . . . . . . .     . . . . . . . │ │ . . O │"
u" ╰───╮ . │ │ . ╭─╮ . ╭─────────────╮ . ╭─╮ . │ │ . ╭───╯"
u" ╭───╯ . ╰─╯ . │ │ . ╰─────╮ ╭─────╯ . │ │ . ╰─╯ . ╰───╮"
u" │ . . . . . . │ │ . . . . │ │ . . . . │ │ . . . . . . │"
u" │ . ╭─────────╯ ╰─────╮ . │ │ . ╭─────╯ ╰─────────╮ . │"
u" │ . ╰─────────────────╯ . ╰─╯ . ╰─────────────────╯ . │"
u" │ . . . . . . . . . . . . . . . . . . . . . . . . . . │"
u" ╰─────────────────────────────────────────────────────╯"
u"                                                        "
u"                                                        "
};


void moveCursor(int x, int y);
void setChar(Canvas* canvas, char16_t c, int x, int y);
void setChars(Canvas* canvas, char16_t c1, char16_t c2, int x, int y);
void setBackground(Canvas* canvas, int color, int x, int y);
void setForeground(Canvas* canvas, int color, int x, int y);
Canvas* createCanvas(int w, int h);
void flushCanvas(Canvas* canvas);

Tile* getTile(Map* map, int x, int y) {
    return &((map->tiles)[(x % map->w) + map->w * (y % map->h)]);
}

Map* createMap(int w, int h) {
    Map* map = malloc(sizeof(Map));
    if (!map) return NULL;
    Tile* ts = (Tile*) malloc((5) * sizeof(Tile));
    if (!ts) return NULL;
    map->food = 0;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int pack = (x + y * w);
            char16_t c2 = mapPreset[pack*2 + 1];

            Tile* t = malloc(sizeof(Tile));
            if (!t) return NULL;
            t->food = c2 == u'O' ? 2 : c2 == u'.' ? 1 : 0;
            t->solid = c2 != u'O' && c2 != u'.' && c2 != u' ';
            map->food += t->food > 0;
            ts[pack] = *t;
        }
    }
    map->w = w;
    map->h = h;
    map->tiles = ts;
    return map;
}

void drawMap(Canvas* canvas, int w, int h, int color) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int pack = (x + y * w)*2;
            char16_t c1 = mapPreset[pack];
            char16_t c2 = mapPreset[pack+1];
            if (c2 == u'.') c2 = u'·';
            if (c2 == u'O') c2 = u'●';
            setChars(canvas, c1, c2, x, y);

            if (c2 == u'·' || c2 == u'●') {
                setForeground(canvas, Foreground.byellow, x, y);
            } else {
                setForeground(canvas, color, x, y);
            }
        }
    }
}