#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

void irq_install();
void register_interrupt_handler(uint8_t irq, void (*handler)(void));

#endif
