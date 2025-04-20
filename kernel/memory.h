#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>  // For size_t

void init_heap(void);
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif
