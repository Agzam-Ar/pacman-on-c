#pragma once

typedef struct KeyboardState {
    int x;
    int y;
    int exit;
    int debug;
} KeyboardState;


KeyboardState Input;
void setupKeyboard();
void handleKeys();

