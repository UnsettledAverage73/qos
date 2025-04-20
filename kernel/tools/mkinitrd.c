#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define INITRD_MAGIC 0xBF

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

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <output file> <input directory>\n", argv[0]);
        return 1;
    }
    
    char* output_file = argv[1];
    char* input_dir = argv[2];
    
    // Open the directory
    DIR* dir = opendir(input_dir);
    if (!dir) {
        perror("Failed to open input directory");
        return 1;
    }
    
    // Count files
    struct dirent* entry;
    uint32_t num_files = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {  // Regular file
            num_files++;
        }
    }
    
    // Allocate headers
    initrd_header_t header = { .nfiles = num_files };
    initrd_file_header_t* file_headers = calloc(num_files, sizeof(initrd_file_header_t));
    
    // Open output file
    FILE* out = fopen(output_file, "wb");
    if (!out) {
        perror("Failed to open output file");
        closedir(dir);
        free(file_headers);
        return 1;
    }
    
    // Write initial header (will update later)
    fwrite(&header, sizeof(header), 1, out);
    
    // Calculate base offset for file data
    uint32_t current_offset = sizeof(header) + num_files * sizeof(initrd_file_header_t);
    
    // Reset directory
    rewinddir(dir);
    
    // Process files
    uint32_t file_index = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", input_dir, entry->d_name);
            
            // Get file size
            struct stat st;
            if (stat(full_path, &st) != 0) {
                perror("Failed to stat file");
                continue;
            }
            
            // Fill header
            file_headers[file_index].magic = INITRD_MAGIC;
            strncpy(file_headers[file_index].name, entry->d_name, 63);
            file_headers[file_index].offset = current_offset;
            file_headers[file_index].length = st.st_size;
            
            // Update offset for next file
            current_offset += st.st_size;
            
            file_index++;
        }
    }
    
    // Write file headers
    fwrite(file_headers, sizeof(initrd_file_header_t), num_files, out);
    
    // Write file data
    rewinddir(dir);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", input_dir, entry->d_name);
            
            // Open and copy file
            FILE* in = fopen(full_path, "rb");
            if (!in) {
                perror("Failed to open input file");
                continue;
            }
            
            char buffer[4096];
            size_t bytes;
            while ((bytes = fread(buffer, 1, sizeof(buffer), in)) > 0) {
                fwrite(buffer, 1, bytes, out);
            }
            
            fclose(in);
        }
    }
    
    // Clean up
    fclose(out);
    closedir(dir);
    free(file_headers);
    
    printf("Created initrd image '%s' with %u files\n", output_file, num_files);
    return 0;
} 