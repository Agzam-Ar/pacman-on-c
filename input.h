#pragma once

typedef struct KeyboardState {
    int x;
    int y;
} KeyboardState;


KeyboardState Input;
void setupKeyboard();
void handleKeys();

