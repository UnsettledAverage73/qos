OUTPUT_FORMAT(elf32-i386)
ENTRY(kernel_main)

SECTIONS {
    /* Start the kernel at 1MB */
    . = 1M;

    /* Text section: code */
    .text : {
        *(.text)
    }

    /* Data section: initialized global/static variables */
    .data : {
        *(.data)
    }

    /* BSS section: uninitialized data (zeroed at runtime) */
    .bss : {
        *(.bss)
        *(COMMON)
    }
}
