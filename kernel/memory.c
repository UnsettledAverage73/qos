#include "memory.h"
#include <stdint.h>

#define HEAP_SIZE 0x100000  // 1MB heap
#define ALIGN 8  // 8-byte alignment
#define HEADER_SIZE sizeof(block_header_t)

typedef struct block_header {
    size_t size;
    uint8_t is_free;
    struct block_header* next;
} block_header_t;

static char heap[HEAP_SIZE] __attribute__((aligned(ALIGN)));
static block_header_t* head = NULL;

static void* align_up(void* addr) {
    return (void*)(((uintptr_t)addr + (ALIGN-1)) & ~(ALIGN-1));
}

void init_heap(void) {
    head = (block_header_t*)heap;
    head->size = HEAP_SIZE - HEADER_SIZE;
    head->is_free = 1;
    head->next = NULL;
}

void* kmalloc(size_t size) {
    if (!head) {
        init_heap();
    }

    // Align the requested size
    size = (size + (ALIGN-1)) & ~(ALIGN-1);

    block_header_t* current = head;
    while (current) {
        if (current->is_free && current->size >= size) {
            // Split block if it's too large
            if (current->size > size + HEADER_SIZE + ALIGN) {
                block_header_t* new_block = (block_header_t*)((char*)current + HEADER_SIZE + size);
                new_block->size = current->size - size - HEADER_SIZE;
                new_block->is_free = 1;
                new_block->next = current->next;
                
                current->size = size;
                current->next = new_block;
            }
            
            current->is_free = 0;
            return (void*)((char*)current + HEADER_SIZE);
        }
        current = current->next;
    }
    
    return NULL;  // Out of memory
}

void kfree(void* ptr) {
    if (!ptr) return;
    
    block_header_t* header = (block_header_t*)((char*)ptr - HEADER_SIZE);
    header->is_free = 1;

    // Coalesce with next block if it's free
    if (header->next && header->next->is_free) {
        header->size += HEADER_SIZE + header->next->size;
        header->next = header->next->next;
    }

    // Coalesce with previous block if it's free
    block_header_t* current = head;
    while (current && current->next != header) {
        current = current->next;
    }
    
    if (current && current->is_free) {
        current->size += HEADER_SIZE + header->size;
        current->next = header->next;
    }
}
