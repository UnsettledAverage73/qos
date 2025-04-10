// kernel/kernel.c

#include "screen.h"
#include "keyboard.h"
#include "string.h"
#include "task.h"

// Simulated task functions
void task1() {
    while (1) {
        print("Running Task 1...\n");
        for (int i = 0; i < 1000000; i++); // Delay loop
    }
}

void task2() {
    while (1) {
        print("Running Task 2...\n");
        for (int i = 0; i < 1000000; i++); // Delay loop
    }
}

void kernel_main() {
    // Setup stack pointer
    asm volatile("mov $0x9000, %esp");
    
    clear_screen();
    print("Welcome to QOS - Quantum Operating System\n");
    print("Initializing task system...\n");

    // Initialize task system once at startup
    init_tasking();
    create_task(task1);
    create_task(task2);

    print("System ready\n");

    while (1) {
        // Simple round-robin scheduling
        schedule();
        
        // Small delay to prevent flooding the screen
        for (volatile int i = 0; i < 1000000; i++);
    }
}
