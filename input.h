#pragma once

typedef struct KeyboardState {
    int x;
    int y;
    int exit;
} KeyboardState;


KeyboardState Input;
void setupKeyboard();
void handleKeys();

