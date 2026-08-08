/**
 * @file memory.h
 * @brief Physical Memory Management and Configuration Header
 * 
 * Operating System Concept:
 * Physical memory (RAM) is divided into fixed-size contiguous blocks called 'Frames'.
 * Logical memory used by processes is divided into same-sized blocks called 'Pages'.
 * The memory management unit (MMU) maps pages to physical frames.
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>

#define MAX_FRAMES 256
#define MAX_PROCESSES 16

/**
 * @struct Frame
 * @brief Represents a single physical memory frame.
 */
typedef struct {
    int frame_id;       /* Physical Frame Index (0, 1, 2, ...) */
    bool is_allocated;  /* True if occupied by a page, false if free */
    int process_id;     /* Owner Process ID (-1 if free) */
    int page_number;    /* Logical Page Number stored in this frame (-1 if free) */
} Frame;

/**
 * @struct MemoryConfig
 * @brief Holds system physical memory configurations and frame state.
 */
typedef struct {
    int total_memory_kb;  /* Total physical memory in Kilobytes (e.g. 1024 KB) */
    int page_size_kb;     /* Fixed size of each frame/page in KB (e.g. 64 KB) */
    int total_frames;     /* Calculated as (total_memory_kb / page_size_kb) */
    int free_frames;      /* Count of currently unallocated frames */
    int num_processes;    /* Number of active processes in simulator */
    Frame frames[MAX_FRAMES]; /* Array of physical frames */
} MemoryConfig;

/* Global or shared memory instance initializer */
void init_memory_config(MemoryConfig *config, int total_mem_kb, int page_sz_kb, int num_procs);

/* Displays memory layout and frame status */
void display_memory_config(const MemoryConfig *config);

/* Checks if page size is valid (power of 2, <= total memory, > 0) */
bool validate_memory_params(int total_mem_kb, int page_sz_kb, int num_procs);

/* Reset/clear frame allocations */
void reset_memory_frames(MemoryConfig *config);

/* Find next free frame index, returns -1 if full */
int get_free_frame(const MemoryConfig *config);

/* Allocate a frame to a process page */
bool allocate_frame(MemoryConfig *config, int frame_id, int proc_id, int page_num);

/* Free an allocated frame */
void deallocate_frame(MemoryConfig *config, int frame_id);

#endif /* MEMORY_H */
