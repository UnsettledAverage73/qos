// kernel/task.c

#include "task.h"  // Contains MAX_TASKS definition
#include "string.h"
#include "screen.h"
#include "ports.h"

task_t *current_task = 0;
task_t *ready_queue = 0;

task_t task_list[MAX_TASKS];
int next_pid = 1;  // Start PIDs from 1

// Helper to allocate a task structure
task_t* get_free_task() {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (!task_list[i].used) {
            task_list[i].used = 1;
            task_list[i].pid = next_pid++;
            return &task_list[i];
        }
    }
    return 0; // No free slot
}

// Initialize multitasking (just 1 task to start)
void init_tasking() {
    current_task = get_free_task();
    if (!current_task) {
        print("Error: Could not initialize tasking\n");
        return;
    }

    current_task->esp = 0;
    current_task->eip = 0;
    current_task->next = 0;
    // pid and used already set by get_free_task()

    ready_queue = current_task;
    print("Tasking initialized\n");
}

// Simple task creation (simulated)
task_t* create_task(void (*entry)()) {
    task_t* task = get_free_task();
    if (!task) return 0;

    task->eip = (uint32_t)entry;
    task->esp = 0;  // Would be the top of the stack in real case
    task->next = 0;

    // Append to ready queue
    task_t* temp = ready_queue;
    while (temp->next) temp = temp->next;
    temp->next = task;

    print("Created task ");
    char pid_str[10];
    int_to_ascii(task->pid, pid_str);
    print(pid_str);
    print("\n");

    return task;
}

// Very basic round-robin scheduler
void switch_task() {
    if (!current_task || !current_task->next) return;

    task_t* next = current_task->next;
    current_task = next;

    // In real OS, save registers, load next task’s context
    print("Switched to task ");
    char pid_str[10];
    int_to_ascii(current_task->pid, pid_str);
    print(pid_str);
    print("\n");
}
