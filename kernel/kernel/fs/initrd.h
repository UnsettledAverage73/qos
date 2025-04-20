#ifndef INITRD_H
#define INITRD_H

#include "vfs.h"
#include <stdint.h>

// InitRD file header
typedef struct {
    uint32_t nfiles;  // Number of files in the ramdisk
} initrd_header_t;

// InitRD file entry
typedef struct {
    uint8_t magic;    // Magic number for verification
    char name[64];    // Filename
    uint32_t offset;  // Offset in the initrd
    uint32_t length;  // Length of the file
} initrd_file_header_t;

// Initialize the initial ramdisk
vfs_node_t* initrd_init(uint32_t location);

// Register the initrd filesystem
void initrd_register(void);

#endif 