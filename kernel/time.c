#include "timer.h"
#include "ports.h"
#include "screen.h"

unsigned int tick = 0;

void timer_callback() {
    tick++;
    print("Tick: ");
    // You can make a function to print numbers later
}

void init_timer(unsigned int frequency) {
    int divisor = 1193180 / frequency;

    outb(0x43, 0x36);             // Command byte
    outb(0x40, divisor & 0xFF);   // Low byte
    outb(0x40, (divisor >> 8));   // High byte
}
