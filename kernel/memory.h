#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>  // For size_t

void* kmalloc(size_t size);
void kfree(void* ptr);

#endif
