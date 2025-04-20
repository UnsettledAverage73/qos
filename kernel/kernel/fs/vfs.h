#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stddef.h>

// File types
#define VFS_FILE        0x01
#define VFS_DIRECTORY   0x02
#define VFS_CHARDEVICE  0x03
#define VFS_BLOCKDEVICE 0x04
#define VFS_PIPE        0x05
#define VFS_SYMLINK     0x06
#define VFS_MOUNTPOINT  0x08

// File permissions
#define VFS_READ    0x04
#define VFS_WRITE   0x02
#define VFS_EXECUTE 0x01

// Forward declarations
struct vfs_node;
struct vfs_filesystem;

// Function pointer types for filesystem operations
typedef struct vfs_node* (*read_type_t)(struct vfs_node*, uint32_t);
typedef void (*write_type_t)(struct vfs_node*, uint32_t);
typedef void (*open_type_t)(struct vfs_node*);
typedef void (*close_type_t)(struct vfs_node*);
typedef struct dirent* (*readdir_type_t)(struct vfs_node*, uint32_t);
typedef struct vfs_node* (*finddir_type_t)(struct vfs_node*, char* name);

// VFS node structure
typedef struct vfs_node {
    char name[256];               // Filename
    uint32_t mask;               // Permission mask
    uint32_t uid;                // User id
    uint32_t gid;                // Group id
    uint32_t flags;              // Includes node type
    uint32_t inode;              // Inode number
    uint32_t length;             // Size of file
    uint32_t impl;               // Implementation-specific data

    // Function pointers for operations
    read_type_t read;
    write_type_t write;
    open_type_t open;
    close_type_t close;
    readdir_type_t readdir;
    finddir_type_t finddir;
    
    struct vfs_node* ptr;        // Used for mountpoints and symlinks
    struct vfs_filesystem* fs;    // Filesystem this node belongs to
} vfs_node_t;

// Directory entry structure
typedef struct dirent {
    char name[256];              // Filename
    uint32_t ino;                // Inode number
} dirent_t;

// Filesystem structure
typedef struct vfs_filesystem {
    char name[32];               // Filesystem name
    uint32_t flags;              // Filesystem flags
    
    // Filesystem operations
    vfs_node_t* (*mount)(const char* device, const char* mountpoint);
    void (*unmount)(vfs_node_t* mountpoint);
    
    struct vfs_filesystem* next;  // Next filesystem in list
} vfs_filesystem_t;

// VFS functions
void vfs_init(void);
int vfs_mount(const char* device, const char* mountpoint, const char* fs_type);
int vfs_unmount(const char* mountpoint);
vfs_node_t* vfs_lookup(const char* path);

// File operations
uint32_t vfs_read(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer);
uint32_t vfs_write(vfs_node_t* node, uint32_t offset, uint32_t size, uint8_t* buffer);
void vfs_open(vfs_node_t* node, uint32_t flags);
void vfs_close(vfs_node_t* node);
struct dirent* vfs_readdir(vfs_node_t* node, uint32_t index);
vfs_node_t* vfs_finddir(vfs_node_t* node, char* name);

// Root node
extern vfs_node_t* vfs_root;

#endif 