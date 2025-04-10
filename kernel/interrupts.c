#include "interrupts.h"
#include "time.h"

extern void init_timer();

// Existing interrupt handling code...

void interrupts_init() {
    // Existing initialization code...
    init_timer(); // Start the timer
    // Rest of initialization...
}
