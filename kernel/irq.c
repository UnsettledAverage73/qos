#include "irq.h"
#include "isr.h"
#include "ports.h"
#include "screen.h"
#include "string.h"  // For int_to_ascii if needed

// IRQ handler function pointers array for IRQ0 to IRQ15
void* irq_routines[16] = { 0 };

// Register a custom IRQ handler
void register_irq_handler(int irq, void (*handler)(registers_t)) {
    irq_routines[irq] = handler;
}

// Unregister a custom IRQ handler (optional utility)
void unregister_irq_handler(int irq) {
    irq_routines[irq] = 0;
}

// Central IRQ handler called from irq.asm stubs
void irq_handler(registers_t regs) {
    int irq = regs.int_no - 32;

    // Call the registered handler if it exists
    if (irq_routines[irq]) {
        void (*handler)(registers_t) = irq_routines[irq];
        handler(regs);
    } else {
        print("⚠ Unhandled IRQ: ");
        char irq_str[3];
        int_to_ascii(regs.int_no, irq_str);
        print(irq_str);
        print("\n");
    }

    // Send End of Interrupt (EOI) to the correct PICs
    if (regs.int_no >= 40) {
        outb(0xA0, 0x20); // Send EOI to slave PIC
    }
    outb(0x20, 0x20);     // Send EOI to master PIC
}
