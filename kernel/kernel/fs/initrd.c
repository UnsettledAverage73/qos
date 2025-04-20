#include "initrd.h"
#include "../memory.h"
#include "../string.h"

#define INITRD_MAGIC 0xBF

// InitRD filesystem instance
static vfs_filesystem_t initrd_fs = {
    .name = "initrd",
    .mount = initrd_mount,
    .unmount = initrd_unmount
};

// Internal file structure
typedef struct {
    initrd_file_header_t header;
    uint8_t* data;
} initrd_file_t;

// Root directory structure
typedef struct {
    initrd_file_t* files;
    uint32_t num_files;
} initrd_root_t;

// Read from an initrd file
static uint32_t initrd_read(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer) {
    initrd_file_t* file = (initrd_file_t*)node->impl;
    
    if (offset >= file->header.length)
        return 0;
        
    if (offset + size > file->header.length)
        size = file->header.length - offset;
        
    memcpy(buffer, file->data + offset, size);
    return size;
}

// Read directory entries
static struct dirent* initrd_readdir(vfs_node_t* node, uint32_t index) {
    initrd_root_t* root = (initrd_root_t*)node->impl;
    
    if (index >= root->num_files)
        return NULL;
        
    static struct dirent entry;
    strcpy(entry.name, root->files[index].header.name);
    entry.ino = index;
    
    return &entry;
}

// Find a file in the directory
static vfs_node_t* initrd_finddir(vfs_node_t* node, char* name) {
    initrd_root_t* root = (initrd_root_t*)node->impl;
    
    for (uint32_t i = 0; i < root->num_files; i++) {
        if (strcmp(name, root->files[i].header.name) == 0) {
            vfs_node_t* file_node = kmalloc(sizeof(vfs_node_t));
            strcpy(file_node->name, root->files[i].header.name);
            file_node->mask = 0555;  // r-xr-xr-x
            file_node->uid = 0;
            file_node->gid = 0;
            file_node->flags = VFS_FILE;
            file_node->inode = i;
            file_node->length = root->files[i].header.length;
            file_node->impl = &root->files[i];
            file_node->read = initrd_read;
            file_node->write = NULL;  // Read-only filesystem
            file_node->readdir = NULL;
            file_node->finddir = NULL;
            file_node->open = NULL;
            file_node->close = NULL;
            file_node->ptr = NULL;
            return file_node;
        }
    }
    
    return NULL;
}

// Mount an initrd filesystem
static vfs_node_t* initrd_mount(const char* device, const char* mountpoint) {
    uint32_t location = strtoul(device, NULL, 16);  // Device is the memory location
    
    // Read the header
    initrd_header_t* header = (initrd_header_t*)location;
    initrd_file_header_t* file_headers = (initrd_file_header_t*)(location + sizeof(initrd_header_t));
    
    // Create root directory
    initrd_root_t* root = kmalloc(sizeof(initrd_root_t));
    root->files = kmalloc(sizeof(initrd_file_t) * header->nfiles);
    root->num_files = header->nfiles;
    
    // Initialize the root directory
    vfs_node_t* root_node = kmalloc(sizeof(vfs_node_t));
    strcpy(root_node->name, "initrd");
    root_node->mask = 0555;  // r-xr-xr-x
    root_node->uid = 0;
    root_node->gid = 0;
    root_node->flags = VFS_DIRECTORY;
    root_node->inode = 0;
    root_node->length = 0;
    root_node->impl = root;
    root_node->read = NULL;
    root_node->write = NULL;
    root_node->readdir = initrd_readdir;
    root_node->finddir = initrd_finddir;
    root_node->open = NULL;
    root_node->close = NULL;
    root_node->ptr = NULL;
    
    // Load files
    uint32_t offset = location + sizeof(initrd_header_t) + header->nfiles * sizeof(initrd_file_header_t);
    for (uint32_t i = 0; i < header->nfiles; i++) {
        // Verify magic number
        if (file_headers[i].magic != INITRD_MAGIC) {
            continue;
        }
        
        // Copy file header
        root->files[i].header = file_headers[i];
        root->files[i].data = (uint8_t*)(offset + file_headers[i].offset);
    }
    
    return root_node;
}

// Unmount an initrd filesystem
static void initrd_unmount(vfs_node_t* mountpoint) {
    if (!mountpoint) return;
    
    // Free the root directory structure
    initrd_root_t* root = (initrd_root_t*)mountpoint->impl;
    if (root) {
        if (root->files) {
            kfree(root->files);
        }
        kfree(root);
    }
    
    // Free the root node
    kfree(mountpoint);
}

// Register the initrd filesystem
void initrd_register(void) {
    vfs_register_filesystem(&initrd_fs);
} 