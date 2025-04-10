#include "scheduler.h"
#include "screen.h"
#include "ports.h"
#include "task.h"

static task_t* task_queue[MAX_TASKS];
static int current_task_idx = -1;
static int total_tasks = 0;
static int ticks_remaining = TIME_QUANTUM;

void init_scheduler() {
    for (int i = 0; i < MAX_TASKS; i++) {
        task_queue[i] = 0;
    }
    current_task_idx = -1;
    total_tasks = 0;
    ticks_remaining = TIME_QUANTUM;
}

void add_task(task_t* task) {
    if (total_tasks < MAX_TASKS) {
        task_queue[total_tasks++] = task;
        if (current_task_idx == -1) {
            current_task_idx = 0;
        }
    }
}

void switch_to_task(task_t* next) {
    if (!next) return;
    
    // Save current task state if it exists
    if (current_task_idx >= 0 && task_queue[current_task_idx]) {
        task_queue[current_task_idx]->state = TASK_READY;
    }
    
    // Switch to new task
    next->state = TASK_RUNNING;
    asm volatile("cli");
    context_switch(
        current_task_idx >= 0 ? &task_queue[current_task_idx]->esp : 0,
        &next->esp
    );
    asm volatile("sti");
}

void schedule() {
    if (total_tasks == 0) return;

    // Check if current task's time quantum expired
    if (--ticks_remaining <= 0) {
        ticks_remaining = TIME_QUANTUM;
        current_task_idx = (current_task_idx + 1) % total_tasks;
    }

    // Skip blocked/sleeping tasks
    int original_idx = current_task_idx;
    while (task_queue[current_task_idx]->state != TASK_READY) {
        current_task_idx = (current_task_idx + 1) % total_tasks;
        if (current_task_idx == original_idx) {
            // No ready tasks found
            return;
        }
    }

    switch_to_task(task_queue[current_task_idx]);
}
