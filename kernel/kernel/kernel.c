#include "screen.h"
#include "keyboard.h"
#include "string.h"
#include "task.h"
#include "scheduler.h"
#include "interrupts.h"
#include "memory.h"
#include "fs/vfs.h"
#include "fs/initrd.h"

// Forward declarations
void interrupts_init();
void task_sleep(uint32_t ticks);

// Test task functions
void task1() {
    int count = 0;
    while (1) {
        print("Task 1 (");
        char num[10];
        int_to_ascii(count++, num);
        print(num);
        print(")\n");
        task_sleep(50); // Sleep for ~1 second
    }
}

void task2() {
    int count = 0;
    while (1) {
        print("  Task 2 (");
        char num[10];
        int_to_ascii(count++, num);
        print(num);
        print(")\n");
        task_sleep(30); // Sleep for ~0.6 seconds
    }
}

// Multiboot structure from GRUB
struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    // ... other fields we don't need right now
};

struct module_info {
    uint32_t mod_start;
    uint32_t mod_end;
    uint32_t string;
    uint32_t reserved;
};

void kernel_main(struct multiboot_info* mboot_ptr) {
    clear_screen();
    print("QOS Kernel Initializing...\n\n");

    // Initialize systems
    init_heap();        // Initialize memory management
    init_scheduler();   // Initialize scheduler
    interrupts_init();  // Set up timer interrupts
    init_tasking();     // Initialize task system
    
    // Initialize filesystem
    vfs_init();
    initrd_register();
    
    // Mount initrd if available
    if (mboot_ptr->mods_count > 0) {
        struct module_info* mod = (struct module_info*)mboot_ptr->mods_addr;
        char location[32];
        snprintf(location, sizeof(location), "%x", mod->mod_start);
        
        print("Mounting initrd from location 0x");
        print(location);
        print("\n");
        
        if (vfs_mount(location, "/", "initrd") == 0) {
            print("InitRD mounted successfully\n");
            
            // Test filesystem by listing root directory
            vfs_node_t* root = vfs_lookup("/");
            if (root) {
                print("Root directory contents:\n");
                struct dirent* entry;
                int index = 0;
                while ((entry = vfs_readdir(root, index++))) {
                    print("  - ");
                    print(entry->name);
                    print("\n");
                }
            }
        } else {
            print("Failed to mount InitRD\n");
        }
    } else {
        print("No InitRD found\n");
    }

    // Create test tasks
    create_task(task1);
    create_task(task2);

    print("\nSystem initialized. Starting tasks...\n\n");

    // Enable interrupts and start scheduling
    asm volatile("sti");

    // Main idle loop
    while (1) {
        asm volatile("hlt");  // Halt CPU when idle
    }
} 