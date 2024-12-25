#include <stdio.h>
#include <uchar.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#include <process.h>
#include <conio.h>
#include "input.h"

void handleKeys() {
    int arrowDown = 80;
    int arrowUp = 72;
    char key = _getch();
    if (key == 'd' || key == 'l' || key == 'в' || key == 'д') { Input.x = +1; Input.y = 0; }
    if (key == 'a' || key == 'j' || key == 'ф' || key == 'о') { Input.x = -1; Input.y = 0; }

    if (key == 's' || key == 'k' || key == 'ы' || key == 'л') { Input.x = 0; Input.y = +1; }
    if (key == 'w' || key == 'i' || key == 'ц' || key == 'ш') { Input.x = 0; Input.y = -1; }
    if (key == 'e') { Input.exit = 1; }
    if (key == 't') { Input.debug = !Input.debug; }
}

static void* keylistener(void* arg) {
    while (1) {
        handleKeys();
    }
}

void setupKeyboard() {
    Input.x = 0;
    Input.y = 0;
    Input.exit = 0;
    Input.debug = 0;
    int tid = _beginthread(&keylistener, 4096, 0);
}