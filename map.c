#include "map.h"
#include "canvas.h"
#include "ghost.h"
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
u"           │ . │ │          b          │ │ . │          "
u"           │ . │ │   ╔═════━━━═════╗   │ │ . │          "
u" ──────────╯ . ╰─╯   ║ #####  #### ║   ╰─╯ . ╰──────────"
u"             .       ║ #i ??p ??c  ║       .            "
u" ──────────╮ . ╭─╮   ║ ########### ║   ╭─╮ . ╭──────────"
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
    return &((map->tiles)[(((x % map->w)+ map->w)% map->w) + map->w * (y % map->h)]);
}

Map* createMap(int w, int h) {
    Map* map = malloc(sizeof(Map));
    if (!map) return NULL;
    Tile* ts = (Tile*) malloc((w*h) * sizeof(Tile));
    if (!ts) return NULL;
    map->food = 0;
    map->score = 0;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int pack = (x + y * w);
            char16_t c1 = mapPreset[pack * 2];
            char16_t c2 = mapPreset[pack*2 + 1];

            if (c1 == u'b') {
                blinky.x = x;
                blinky.y = y;
                c1 = u' ';
            }
            if (c1 == u'p') {
                pinky.x = x;
                pinky.y = y;
                c1 = u' ';
            }
            if (c1 == u'i') {
                inky.x = x;
                inky.y = y;
                c1 = u' ';
            }
            if (c1 == u'c') {
                clyde.x = x;
                clyde.y = y;
                c1 = u' ';
            }
            char solid = c2 != u'O' && c2 != u'.' && c2 != u' ';
            if (c1 == u'━' && c2 == u'━') solid = 2;
            if (c1 == u'?' && c2 == u'?') solid = 2;


            Tile* t = malloc(sizeof(Tile));
            if (!t) return NULL;
            t->food = c2 == u'O' ? 2 : c2 == u'.' ? 1 : 0;
            t->solid = solid;
            map->food += t->food > 0;
            ts[pack] = *t;

        }
    }
    map->w = w;
    map->h = h;
    map->tiles = ts;
    map->maxFood = map->food;
    return map;
}

void drawMap(Canvas* canvas, int w, int h, int color, char full) {
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            int pack = (x + y * w)*2;
            char16_t c1 = mapPreset[pack];
            char16_t c2 = mapPreset[pack+1];
            if (c2 == u'.') c2 = full ? u'·' : u' ';
            if (c2 == u'O') c2 = full ? u'●' : u' ';
            if (c1 == u'b') c1 = u' ';
            if (c1 == u'p') c1 = u' ';
            if (c1 == u'i') c1 = u' ';
            if (c1 == u'c') c1 = u' ';
            if (c1 == u'#') c1 = u' ';
            if (c1 == u'?') c1 = u' ';
            if (c2 == u'#') c2 = u' ';
            if (c2 == u'?') c2 = u' ';
            setChars(canvas, c1, c2, x, y);
            if (c2 == u'·' || c2 == u'●') {
                setForeground(canvas, Color.coral, x, y);
            } else {
                setForeground(canvas, color, x, y);
            }
        }
    }
}

void drawTile(Canvas* canvas, Map* map, int x, int y) {
    Tile* t = getTile(map, x, y);
    if (t->solid > 0) {
        int pack = (x + y * map->w) * 2;
        char16_t c1 = mapPreset[pack];
        char16_t c2 = mapPreset[pack + 1];
        if (c1 == u'?') c1 = u' ';
        if (c2 == u'?') c2 = u' ';
        setChars(canvas, c1, c2, x, y);
        return;
    }

    if (t->food == 0) setChars(canvas, u' ', u' ', x, y);
    if (t->food == 1) setChars(canvas, u' ', u'·', x, y);
    if (t->food == 2) setChars(canvas, u' ', u'●', x, y);
    if (t->food > 0) setForeground(canvas, Color.coral, x, y);
}