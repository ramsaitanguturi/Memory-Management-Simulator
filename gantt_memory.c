/**
 * @file gantt_memory.c
 * @brief Implementation of ASCII Memory Frame Visualizer
 */

#include <stdio.h>
#include "gantt_memory.h"
#include "utils.h"

/**
 * Displays ASCII memory frame status line by line with address boundaries.
 */
void display_ascii_memory_frames(const MemoryConfig *config) {
    print_header("ADDITIONAL FEATURE: ASCII PHYSICAL MEMORY FRAME VISUALIZATION");

    printf(" Memory Map Overview (Frame Size = %d KB):\n\n", config->page_size_kb);

    for (int f = 0; f < config->total_frames; f++) {
        int start_addr = f * config->page_size_kb;
        int end_addr = start_addr + config->page_size_kb;

        if (config->frames[f].is_allocated) {
            printf(" Frame %-2d : Process P%-2d (Page %-2d)  [Range: %4d KB - %4d KB]\n",
                   f,
                   config->frames[f].process_id,
                   config->frames[f].page_number,
                   start_addr,
                   end_addr);
        } else {
            printf(" Frame %-2d : Empty                [Range: %4d KB - %4d KB]\n",
                   f,
                   start_addr,
                   end_addr);
        }
    }
    printf("\n");
    print_separator();
}

/**
 * Displays horizontal visual box representation of RAM layout.
 */
void display_memory_map_boxes(const MemoryConfig *config) {
    printf(" Visual RAM Frame Grid Layout:\n\n ");

    /* Top border */
    for (int f = 0; f < config->total_frames; f++) {
        printf("+-------");
    }
    printf("+\n ");

    /* Frame Number header */
    for (int f = 0; f < config->total_frames; f++) {
        printf("| F%-4d ", f);
    }
    printf("|\n ");

    /* Middle border */
    for (int f = 0; f < config->total_frames; f++) {
        printf("+-------");
    }
    printf("+\n ");

    /* Content line */
    for (int f = 0; f < config->total_frames; f++) {
        if (config->frames[f].is_allocated) {
            printf("| P%-2d:Pg%d", config->frames[f].process_id, config->frames[f].page_number);
        } else {
            printf("| Empty ");
        }
    }
    printf("|\n ");

    /* Bottom border */
    for (int f = 0; f < config->total_frames; f++) {
        printf("+-------");
    }
    printf("+\n\n");
}
