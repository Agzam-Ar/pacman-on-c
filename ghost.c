#include "ghost.h"
#include "player.h"
#include "canvas.h"
#include "map.h"
#include "input.h"
#include <uchar.h>
#include <stdlib.h>

// TODO: lock door after ghost exit

void drawTile(Canvas* canvas, Map* map, int x, int y);

static char isChase(Map* map) {
	int time = map->time;
	if (time < 7) return 0;
	if (time < 27) return 1;
	time -= 27;
	if (time < 7) return 0;
	if (time < 27) return 1;
	time -= 27;
	if (time < 5) return 0;
	if (time < 25) return 1;
	time -= 25;
	if (time < 5) return 0;
	return 1;
}

static blinkyTarget(Player* player, Map* map, int* tx, int* ty) {
	if (!isChase(map)) {
		*tx = map->w - 3;
		*ty = 0;
		return;
	}
	*tx = player->x;
	*ty = player->y;
}

static pinkyTarget(Player* player, Map* map, int* tx, int* ty) {
	if (!isChase(map)) {
		*tx = 2;
		*ty = 0;
		return;
	}
	*tx = player->x + player->vx * 4;
	*ty = player->y + player->vy * 4;
}

static inkyTarget(Player* player, Map* map, int* tx, int* ty) {
	if (!isChase(map)) {
		*tx = map->w-1;
		*ty = map->h-1;
		return;
	}
	int food = map->maxFood - map->food;
	if (food >= 30) {
		int vx = player->x + player->vx * 2;
		int vy = player->y + player->vy * 2;

		vx = vx - blinky.x;
		vy = vy - blinky.y;

		*tx = blinky.x + vx * 2;
		*ty = blinky.y + vy * 2;
		inky.atHome = 0;
	} else {
		*tx = map->w / 2 - 2;
		*ty = map->h / 2 - 1;
		inky.atHome = 1;
	}
}

static clydeTarget(Player* player, Map* map, int* tx, int* ty) {
	if (!isChase(map)) {
		*tx = 0;
		*ty = map->h - 1;
		return;
	}
	int food = map->maxFood - map->food;
	if (food >= map->maxFood/3) {
		int dx = player->x - clyde.x;
		int dy = player->y - clyde.y;
		int dst2 = dx * dx + dy * dy;
		if (dst2 > 64) { // 8*8 = 64
			*tx = player->x;
			*ty = player->y;
		} else {
			*tx = 0;
			*ty = map->h-1;
		}
		clyde.atHome = 0;
	} else {
		*tx = map->w / 2 + 2;
		*ty = map->h / 2 - 1;
		clyde.atHome = 1;
	}
}

void loadGhosts() {
	blinky.color = Color.red;
	pinky.color = Color.pink;
	inky.color = Color.cyan;
	clyde.color = Color.orange;

	blinky.getTarget = &blinkyTarget;
	pinky.getTarget = &pinkyTarget;
	inky.getTarget = &inkyTarget;
	clyde.getTarget = &clydeTarget;


	blinky.atHome = 0;
	pinky.atHome = 0;
	inky.atHome = 1;
	clyde.atHome = 1;
}

static int ix[4] = { 1,0,-1,0 };
static int iy[4] = { 0,1,0,-1 };

void updateGhost(Ghost* ghost, Player* player, Canvas* canvas, Map* map) {

	int x = ghost->x;
	int y = ghost->y;

	int tx = 0;
	int ty = 0;
	ghost->getTarget(player, map, &tx, &ty);

	int dx = 0;
	int dy = 0;
	int minDst2 = 1000000;

	for (int i = 0; i < 4; i++) {
		int rx = x + ix[i];
		int ry = y + iy[i];
		if(rx == ghost->lx && ry == ghost->ly) continue;
		Tile* t = getTile(map, rx, ry);
		if (ghost->atHome) {
			if (t->solid) continue;
		} else {
			if (t->solid && t->solid != 2) continue;
		}
		int dstx = tx-rx;
		int dsty = ty-ry;
		int dst2 = dstx * dstx + dsty * dsty;
		dst2 += t->solid*10;
		if (dst2 < minDst2) {
			minDst2 = dst2;
			dx = ix[i];
			dy = iy[i];
		}
	}

	x += dx;
	x = ((x % map->w) + map->w) % map->w;
	y += dy;

	drawTile(canvas, map, ghost->x, ghost->y);
	setChars(canvas, u' ', u'&', x, y);
	setForeground(canvas, ghost->color, x, y);
	if(Input.debug) setBackground(canvas, ghost->color, tx, ty);

	ghost->lx = ghost->x;
	ghost->ly = ghost->y;
	ghost->x = x;
	ghost->y = y;

}