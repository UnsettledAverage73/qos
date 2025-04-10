#ifndef TASK_H
#define TASK_H

#include <stdint.h>

#define MAX_TASKS 16

typedef enum {
    TASK_READY,
    TASK_RUNNING, 
    TASK_BLOCKED,
    TASK_TERMINATED
} task_state_t;

typedef struct task {
    uint32_t esp;          // Stack pointer
    uint32_t ebp;          // Base pointer
    uint32_t eip;          // Instruction pointer
    struct task *next;     // Next task in queue
    int pid;               // Process ID
    char name[32];         // Task name
    task_state_t state;    // Current state
    uint32_t sleep_until;  // Tick when task should wake up
    int used;              // Is task slot used?
} task_t;

// Function prototypes
void init_tasking();
task_t* create_task(void (*entry)());
void schedule();  // Preemptive scheduler

// Assembly function declarations
extern void context_switch(uint32_t* current_esp, uint32_t* next_esp);

#endif
