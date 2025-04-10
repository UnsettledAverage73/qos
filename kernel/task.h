// kernel/task.h
#ifndef TASK_H
#define TASK_H

#include <stdint.h>

#define MAX_TASKS 16  // Maximum number of tasks

// Task structure
typedef struct task {
    uint32_t esp;          // Stack pointer
    uint32_t ebp;          // Base pointer
    uint32_t eip;          // Instruction pointer
    struct task *next;     // Pointer to next task
    int pid;               // Process ID
    int used;              // Is task slot used?
} task_t;

// Function prototypes
task_t* create_task(void (*entry_point)());
void schedule();
void tasking_tick();
void init_tasking();

#endif // TASK_H