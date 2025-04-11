#include "memory.h"

// Simple placeholder implementation for early development
#define HEAP_SIZE 0x100000  // 1MB heap
static char heap[HEAP_SIZE];
static char* current = heap;

void* kmalloc(size_t size) {
    if (current + size > heap + HEAP_SIZE) return 0;
    void* ptr = current;
    current += size;
    return ptr;
}

void kfree(void* ptr) {
    // Simple allocator doesn't support free
}
