#include "screen.h"
#include "string.h"  // for int_to_ascii

typedef struct {
    unsigned int ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
} registers_t;

void isr_handler(registers_t regs) {
    print("🔔 Interrupt received: ");

    char str[4];  // Enough for interrupt numbers up to 255 (3 digits + null)
    int_to_ascii(regs.int_no, str);
    print(str);
    print("\n");

    // Optional: Display more context
    /*
    print("EIP: ");
    int_to_ascii(regs.eip, str);
    print(str);
    print("\n");
    */

    // Halt the system to prevent weird behavior (for now)
    print("System Halted.\n");
    while (1) {
        asm volatile("hlt");
    }
}

