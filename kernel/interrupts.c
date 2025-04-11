#include "interrupts.h"
#include "time.h"
#include "screen.h"
#include "ports.h"
#include <stdint.h>  // For uint8_t type

#define PIC1_CMD  0x20
#define PIC1_DATA 0x21
#define PIC2_CMD  0xA0
#define PIC2_DATA 0xA1

// IRQ handler function pointers
static void (*irq_handlers[16])(void);

void register_interrupt_handler(uint8_t irq, void (*handler)(void)) {
    irq_handlers[irq] = handler;
}

// Internal IRQ handler that calls the registered handler
static void internal_irq_handler(uint8_t irq) {
    if (irq_handlers[irq]) {
        irq_handlers[irq]();
    }
    // Send EOI to PIC
    if (irq >= 8) {
        outb(PIC2_CMD, 0x20);
    }
    outb(PIC1_CMD, 0x20);
}

void interrupts_init() {
    print("Initializing interrupts...\n");
    
    // Initialize PICs
    outb(PIC1_DATA, 0x11);  // ICW1: edge triggered, cascade, ICW4 needed
    outb(PIC2_DATA, 0x11);
    outb(PIC1_DATA, 0x20);  // ICW2: IRQ0-7 mapped to 0x20-0x27
    outb(PIC2_DATA, 0x28);  // ICW2: IRQ8-15 mapped to 0x28-0x2F
    outb(PIC1_DATA, 0x04);  // ICW3: slave PIC at IRQ2
    outb(PIC2_DATA, 0x02);
    outb(PIC1_DATA, 0x01);  // ICW4: 8086 mode
    outb(PIC2_DATA, 0x01);
    
    // Mask all interrupts except IRQ0 (timer)
    outb(PIC1_DATA, 0xFE);  // Only allow IRQ0
    outb(PIC2_DATA, 0xFF);  // Mask all IRQ8-15
    
    // Initialize timer (will register its own handler)
    init_timer();
    
    print("Interrupts initialized\n");
}
