
#include "player.h"
#include "input.h"
#include "canvas.h"
#include "map.h"
#include <uchar.h>
#include <stdlib.h>

// TODO: "CC" for ghosts eye

void moveCursor(int x, int y);
void setChar(Canvas* canvas, char16_t c, int x, int y);
void setChars(Canvas* canvas, char16_t c1, char16_t c2, int x, int y);
void setBackground(Canvas* canvas, int color, int x, int y);
void setForeground(Canvas* canvas, int color, int x, int y);
void flushCanvas(Canvas* canvas);

Tile* getTile(Map* map, int x, int y);

void updatePlayer(Player* player, Canvas* canvas, Map* map) {

    int x = player->x;
    int y = player->y;

    int vx = Input.x;
    int vy = Input.y;

    Tile* t = getTile(map, x+vx, y+vy);
    if (t->solid) {
        vx = player->vx;
        vy = player->vy;
    } else {
        player->vx = vx;
        player->vy = vy;
    }

    x += vx;
    y += vy;

    t = getTile(map, x, y);
    if (t->solid) return;

    if (t->food > 0) {
        map->food--;
        t->food = 0;
    }
    setChar(canvas, u' ', player->x, player->y);


    setChars(canvas, u' ', u'P', x, y);
    setForeground(canvas, Foreground.yellow, x, y);

    player->x = x;
    player->y = y;
}