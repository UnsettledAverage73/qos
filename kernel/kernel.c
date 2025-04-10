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
    clear_screen();
    print("Welcome to QOS - Quantum Operating System\n");
    print("Type 'help' to get started.\n\n");

    // Initialize task system once at startup
    init_tasking();
    create_task(task1);
    create_task(task2);

    char command[100];

    while (1) {
        print("QOS > ");
        int i = 0;
        char c;

        while (1) {
            c = read_char();

            if (c == '\n') {
                command[i] = '\0';
                print("\n");
                break;
            } else if (c == '\b') {
                if (i > 0) {
                    i--;
                    print_char('\b');
                    print_char(' ');
                    print_char('\b');
                }
            } else if (c && i < sizeof(command) - 1) {
                command[i++] = c;
                print_char(c);
            }
        }

        if (strcmp(command, "help") == 0) {
            print("Available commands:\n");
            print("- help  : Show this help message\n");
            print("- clear : Clear the screen\n");
        } else if (strcmp(command, "clear") == 0) {
            clear_screen();
        } else if (strlen(command) == 0) {
            // Do nothing on empty input
        } else {
            print("Unknown command: ");
            print(command);
            print("\n");
        }

        // Perform task switching
        sleep(1);
        switch_task();
    }
}
