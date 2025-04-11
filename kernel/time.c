#include "ports.h"
#include "interrupts.h"
#include "task.h"
#include "screen.h"  // For print()

#define PIT_FREQUENCY 100     // 100 Hz = 10 ms slices
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40

static uint32_t system_ticks = 0;

static void timer_callback() {
    system_ticks++;
    schedule();  // switch tasks on every timer interrupt
}

uint32_t get_ticks() {
    return system_ticks;
}

void init_timer() {
    uint32_t divisor = 1193180 / PIT_FREQUENCY;

    outb(PIT_COMMAND_PORT, 0x36);               // Set command byte
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);    // Low byte
    outb(PIT_CHANNEL0_PORT, divisor >> 8);      // High byte

    register_interrupt_handler(32, timer_callback);  // IRQ0 = interrupt 32
}
