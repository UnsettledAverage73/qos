#ifndef IRQ_H
#define IRQ_H

#include "isr.h"

void irq_handler(registers_t regs);
void register_irq_handler(int irq, void (*handler)(registers_t));

#endif
