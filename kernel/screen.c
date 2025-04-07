#include "screen.h"

#define VIDEO_ADDRESS 0xB8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0F

// Cast VIDEO_MEMORY as a char pointer to access it like an array
volatile char* VIDEO_MEMORY = (volatile char*) VIDEO_ADDRESS;

int cursor_pos = 0;

void print_char(char c) {
    VIDEO_MEMORY[cursor_pos * 2] = c;
    VIDEO_MEMORY[cursor_pos * 2 + 1] = WHITE_ON_BLACK;
    cursor_pos++;
}

void print(char* str) {
    int i = 0;
    while (str[i]) {
        print_char(str[i]);
        i++;
    }
}

void clear_screen() {
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++) {
        VIDEO_MEMORY[i * 2] = ' ';
        VIDEO_MEMORY[i * 2 + 1] = WHITE_ON_BLACK;
    }
    cursor_pos = 0;
}
