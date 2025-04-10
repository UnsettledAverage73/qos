// kernel/kernel.c

#include "screen.h"
#include "keyboard.h"
#include "string.h"
#include "task.h"

// Simulated task functions
void task1() {
    while (1) {
        print("Task 1 running\n");
        for (int i = 0; i < 1000000; i++); // Delay
    }
}

void task2() {
    while (1) {
        print("Task 2 running\n");
        for (int i = 0; i < 1000000; i++); // Delay
    }
}

void kernel_main() {
    clear_screen();
    print("Welcome to QOS with Preemptive Multitasking!\n\n");

    // Initialize systems
    interrupts_init();  // Sets up timer interrupts
    init_tasking();     // Initializes task system

    // Create test tasks
    create_task(task1);
    create_task(task2);

    // Main idle loop - task switching now happens via timer interrupts
    while (1) {
        asm volatile("hlt");  // Halt CPU when idle
    }
}
