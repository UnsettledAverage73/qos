#include "screen.h"
#include "keyboard.h"
#include "string.h"
#include "task.h"
#include "scheduler.h"
#include "interrupts.h"

// Forward declarations
void interrupts_init();
void task_sleep(uint32_t ticks);

// Test task functions
void task1() {
    int count = 0;
    while (1) {
        print("Task 1 (");
        char num[10];
        int_to_ascii(count++, num);
        print(num);
        print(")\n");
        task_sleep(50); // Sleep for ~1 second
    }
}

void task2() {
    int count = 0;
    while (1) {
        print("  Task 2 (");
        char num[10];
        int_to_ascii(count++, num);
        print(num);
        print(")\n");
        task_sleep(30); // Sleep for ~0.6 seconds
    }
}

void kernel_main() {
    clear_screen();
    print("QOS Preemptive Multitasking Test\n\n");

    // Initialize systems
    init_scheduler();    // Initialize scheduler
    interrupts_init();   // Set up timer interrupts
    init_tasking();      // Initialize task system

    // Create test tasks
    create_task(task1);
    create_task(task2);

    print("System initialized. Starting tasks...\n\n");

    // Enable interrupts and start scheduling
    asm volatile("sti");

    // Main idle loop
    while (1) {
        asm volatile("hlt");  // Halt CPU when idle
    }
}
