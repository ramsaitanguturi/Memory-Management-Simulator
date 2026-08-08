/**
 * @file gantt_memory.h
 * @brief ASCII Physical Memory Map & Frame Visualizer Header
 * 
 * Operating System Concept:
 * Visual representation of physical RAM frames showing allocation state,
 * process owners, page numbers, and physical address byte ranges.
 */

#ifndef GANTT_MEMORY_H
#define GANTT_MEMORY_H

#include "memory.h"

/* Displays detailed ASCII frame layout and address bounds */
void display_ascii_memory_frames(const MemoryConfig *config);

/* Displays visual horizontal memory map box diagram */
void display_memory_map_boxes(const MemoryConfig *config);

#endif /* GANTT_MEMORY_H */
