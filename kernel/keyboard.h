// kernel/keyboard.h

#ifndef KEYBOARD_H
#define KEYBOARD_H

// Optional: to setup IRQ1 (keyboard) in future
void init_keyboard();

// Blocking call that waits and returns ASCII character from keyboard
char read_char();

// Raw scancode from the keyboard hardware
char read_scancode();

#endif
