// kernel/task.c

#include "task.h"
#include "string.h"
#include "screen.h"
#include "ports.h"
#include "memory.h"  // For kmalloc()

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
    task_list[i].context.ebp = 0;
    task_list[i].state = TASK_READY;
    strcpy(task_list[i].name, "task");
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

    current_task->context.esp = 0;
    current_task->context.eip = 0;
    current_task->next = 0;
    // pid and used already set by get_free_task()

    ready_queue = current_task;
    print("Tasking initialized\n");
}

// Task creation with proper stack setup
task_t* create_task(void (*entry)()) {
    task_t* task = get_free_task();
    if (!task) return 0;

    // Allocate stack (4KB per task)
    uint32_t* stack = (uint32_t*)kmalloc(4096);
    if (!stack) return 0;

    // Set up initial stack frame
    stack += 1024; // Start at top of stack
    *(--stack) = (uint32_t)entry; // Return address
    *(--stack) = 0x202; // EFLAGS (interrupts enabled)
    *(--stack) = 0x8; // CS
    *(--stack) = 0x0; // EAX, EBX, ECX, EDX, etc.
    *(--stack) = 0x0;
    *(--stack) = 0x0;
    *(--stack) = 0x0;
    *(--stack) = 0x0; // EBP will be set on first switch

    task->context.eip = (uint32_t)entry;
    task->context.esp = (uint32_t)stack;
    task->context.ebp = (uint32_t)stack;
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

// Forward declaration of scheduler's schedule function
void schedule();

#include "time.h"

void task_sleep(uint32_t ticks) {
    if (!current_task) return;
    
    current_task->sleep_until = get_ticks() + ticks;
    current_task->state = TASK_BLOCKED;
    schedule();
}
