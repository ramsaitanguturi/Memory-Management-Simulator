/**
 * @file memory.c
 * @brief Implementation of Physical Memory Configuration & Frame Control
 */

#include <stdio.h>
#include <stdbool.h>
#include "memory.h"
#include "utils.h"

/**
 * Validates memory parameters according to OS architectural rules.
 * Page size must be a power of 2 and must not exceed total physical memory.
 */
bool validate_memory_params(int total_mem_kb, int page_sz_kb, int num_procs) {
    if (total_mem_kb <= 0 || page_sz_kb <= 0 || num_procs < 0) {
        print_error("Memory size, page size, and process count must be positive values.");
        return false;
    }

    if (page_sz_kb > total_mem_kb) {
        print_error("Page size cannot be larger than total physical memory.");
        return false;
    }

    /* Check if page_sz_kb is a power of 2 */
    if ((page_sz_kb & (page_sz_kb - 1)) != 0) {
        print_warning("Page size is not a power of 2! Real OS architectures require power of 2 page sizes.");
    }

    int calculated_frames = total_mem_kb / page_sz_kb;
    if (calculated_frames > MAX_FRAMES) {
        print_error("Calculated frame count exceeds simulator maximum frame capacity.");
        return false;
    }

    return true;
}

/**
 * Initializes physical memory configuration structure.
 */
void init_memory_config(MemoryConfig *config, int total_mem_kb, int page_sz_kb, int num_procs) {
    config->total_memory_kb = total_mem_kb;
    config->page_size_kb = page_sz_kb;
    config->total_frames = total_mem_kb / page_sz_kb;
    config->free_frames = config->total_frames;
    config->num_processes = num_procs;

    for (int i = 0; i < config->total_frames; i++) {
        config->frames[i].frame_id = i;
        config->frames[i].is_allocated = false;
        config->frames[i].process_id = -1;
        config->frames[i].page_number = -1;
    }
}

/**
 * Resets all physical frames to unallocated state.
 */
void reset_memory_frames(MemoryConfig *config) {
    config->free_frames = config->total_frames;
    for (int i = 0; i < config->total_frames; i++) {
        config->frames[i].is_allocated = false;
        config->frames[i].process_id = -1;
        config->frames[i].page_number = -1;
    }
}

/**
 * Finds the index of the first available free frame.
 */
int get_free_frame(const MemoryConfig *config) {
    for (int i = 0; i < config->total_frames; i++) {
        if (!config->frames[i].is_allocated) {
            return i;
        }
    }
    return -1; /* No free frames available */
}

/**
 * Allocates a physical frame to a process page.
 */
bool allocate_frame(MemoryConfig *config, int frame_id, int proc_id, int page_num) {
    if (frame_id < 0 || frame_id >= config->total_frames) {
        return false;
    }

    if (!config->frames[frame_id].is_allocated) {
        config->free_frames--;
    }

    config->frames[frame_id].is_allocated = true;
    config->frames[frame_id].process_id = proc_id;
    config->frames[frame_id].page_number = page_num;
    return true;
}

/**
 * Deallocates a frame.
 */
void deallocate_frame(MemoryConfig *config, int frame_id) {
    if (frame_id >= 0 && frame_id < config->total_frames) {
        if (config->frames[frame_id].is_allocated) {
            config->frames[frame_id].is_allocated = false;
            config->frames[frame_id].process_id = -1;
            config->frames[frame_id].page_number = -1;
            config->free_frames++;
        }
    }
}

/**
 * Displays memory configuration parameters and list of physical frames.
 */
void display_memory_config(const MemoryConfig *config) {
    print_header("PHYSICAL MEMORY CONFIGURATION");
    printf(" Total Physical Memory : %d KB\n", config->total_memory_kb);
    printf(" Page / Frame Size     : %d KB\n", config->page_size_kb);
    printf(" Total Physical Frames : %d\n", config->total_frames);
    printf(" Free Frames           : %d\n", config->free_frames);
    printf(" Allocated Frames      : %d\n", config->total_frames - config->free_frames);
    printf(" Number of Processes   : %d\n", config->num_processes);
    print_separator();

    printf("Frame Numbers:\n");
    for (int i = 0; i < config->total_frames; i++) {
        printf("%d ", i);
    }
    printf("\n\n");
}
