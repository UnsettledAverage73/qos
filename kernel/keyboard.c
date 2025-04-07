// kernel/keyboard.c

#include "keyboard.h"
#include "screen.h"
#include "ports.h"
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60

// US QWERTY keyboard scancode to ASCII map
static char scancode_to_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 0-9 */
    '9', '0', '-', '=', '\b',                      /* Backspace */
    '\t',                                          /* Tab */
    'q', 'w', 'e', 'r',                            /* 16-19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',  /* Enter key */
    0,                                             /* Left Ctrl */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,                                             /* Left Shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',
    0,                                             /* Right Shift */
    '*',
    0,                                             /* Alt */
    ' ',                                           /* Space bar */
    0,                                             /* Caps lock */
    // ... (you can extend this array as needed)
};

// Reads a raw scancode from the keyboard
char read_scancode() {
    return inb(KEYBOARD_DATA_PORT);
}

// Reads a single ASCII character from keyboard (blocking)
char read_char() {
    char scancode = 0;
    char c = 0;
    while (1) {
        scancode = read_scancode();
        if (scancode < sizeof(scancode_to_ascii)) {
            c = scancode_to_ascii[(int)scancode];
            if (c != 0) {
                return c;
            }
        }
    }
}
