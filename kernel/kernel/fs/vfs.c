#include "vfs.h"
#include "../memory.h"
#include "../string.h"

// Global root node
vfs_node_t* vfs_root = NULL;

// List of registered filesystems
static vfs_filesystem_t* filesystems = NULL;

void vfs_init(void) {
    vfs_root = NULL;
    filesystems = NULL;
}

// Register a filesystem type
int vfs_register_filesystem(vfs_filesystem_t* fs) {
    if (!fs || !fs->name[0]) return -1;
    
    // Add to filesystem list
    fs->next = filesystems;
    filesystems = fs;
    return 0;
}

// Find a filesystem by name
static vfs_filesystem_t* find_filesystem(const char* name) {
    vfs_filesystem_t* fs = filesystems;
    while (fs) {
        if (strcmp(fs->name, name) == 0) {
            return fs;
        }
        fs = fs->next;
    }
    return NULL;
}

// Split path into components
static char** split_path(const char* path, int* count) {
    int len = strlen(path);
    int parts = 1;
    for (int i = 0; i < len; i++) {
        if (path[i] == '/') parts++;
    }
    
    char** result = kmalloc(sizeof(char*) * parts);
    *count = 0;
    
    char* start = (char*)path;
    while (*start == '/') start++;
    
    char* end = start;
    while (*end) {
        if (*end == '/') {
            int part_len = end - start;
            if (part_len > 0) {
                result[*count] = kmalloc(part_len + 1);
                memcpy(result[*count], start, part_len);
                result[*count][part_len] = '\0';
                (*count)++;
            }
            start = end + 1;
        }
        end++;
    }
    
    // Handle last part
    if (end > start) {
        int part_len = end - start;
        result[*count] = kmalloc(part_len + 1);
        memcpy(result[*count], start, part_len);
        result[*count][part_len] = '\0';
        (*count)++;
    }
    
    return result;
}

// Mount a filesystem
int vfs_mount(const char* device, const char* mountpoint, const char* fs_type) {
    vfs_filesystem_t* fs = find_filesystem(fs_type);
    if (!fs) return -1;
    
    // Mount the filesystem
    vfs_node_t* mounted = fs->mount(device, mountpoint);
    if (!mounted) return -1;
    
    // If this is the root mount
    if (strcmp(mountpoint, "/") == 0) {
        vfs_root = mounted;
        return 0;
    }
    
    // Find the mountpoint node
    vfs_node_t* mount_node = vfs_lookup(mountpoint);
    if (!mount_node) return -1;
    
    // Set up the mountpoint
    mount_node->flags |= VFS_MOUNTPOINT;
    mount_node->ptr = mounted;
    
    return 0;
}

// Unmount a filesystem
int vfs_unmount(const char* mountpoint) {
    vfs_node_t* node = vfs_lookup(mountpoint);
    if (!node || !(node->flags & VFS_MOUNTPOINT)) return -1;
    
    // Call filesystem specific unmount
    if (node->fs && node->fs->unmount) {
        node->fs->unmount(node);
    }
    
    // Clear mountpoint flag and pointer
    node->flags &= ~VFS_MOUNTPOINT;
    node->ptr = NULL;
    
    return 0;
}

// Look up a path in the VFS
vfs_node_t* vfs_lookup(const char* path) {
    if (!vfs_root) return NULL;
    if (strcmp(path, "/") == 0) return vfs_root;
    
    int count;
    char** parts = split_path(path, &count);
    
    vfs_node_t* current = vfs_root;
    for (int i = 0; i < count; i++) {
        if (!current) break;
        
        // Handle mountpoints
        if (current->flags & VFS_MOUNTPOINT) {
            current = current->ptr;
        }
        
        // Find the next component
        current = vfs_finddir(current, parts[i]);
        
        // Free the path component
        kfree(parts[i]);
    }
    
    // Free the parts array
    kfree(parts);
    
    return current;
}

// Standard file operations
uint32_t vfs_read(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer) {
    if (node && node->read) {
        return node->read(node, offset, size, buffer);
    }
    return 0;
}

uint32_t vfs_write(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer) {
    if (node && node->write) {
        return node->write(node, offset, size, buffer);
    }
    return 0;
}

void vfs_open(vfs_node_t* node, uint32_t flags) {
    if (node && node->open) {
        node->open(node);
    }
}

void vfs_close(vfs_node_t* node) {
    if (node && node->close) {
        node->close(node);
    }
}

struct dirent* vfs_readdir(vfs_node_t* node, uint32_t index) {
    if (node && node->readdir) {
        return node->readdir(node, index);
    }
    return NULL;
}

vfs_node_t* vfs_finddir(vfs_node_t* node, char* name) {
    if (node && node->finddir) {
        return node->finddir(node, name);
    }
    return NULL;
} 