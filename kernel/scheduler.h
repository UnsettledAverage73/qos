#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

#define TIME_QUANTUM 5  // Number of timer ticks per task

void init_scheduler();
void schedule();
void add_task(task_t* task);
void switch_to_task(task_t* next);

#endif
