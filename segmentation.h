/**
 * @file segmentation.h
 * @brief Segmentation Simulation & Segment Table Header
 * 
 * Operating System Concept:
 * Segmentation supports user view of memory. A program is a collection of segments
 * (e.g. main program, procedure, stack, data structures, symbol table).
 * Each segment has a logical name/number, a base physical address, and a length limit.
 * The CPU MMU checks that Offset < Limit; otherwise, a Segmentation Fault is raised.
 */

#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <stdbool.h>

#define MAX_SEGMENTS 32

/**
 * @struct SegmentTableEntry
 * @brief Single segment descriptor.
 */
typedef struct {
    int segment_id;      /* Segment number (0, 1, 2, ...) */
    char name[32];       /* Segment label e.g., "Code", "Data", "Stack" */
    int base_address;    /* Physical starting address in RAM */
    int limit;           /* Length/size of the segment */
    bool valid;          /* Valid descriptor */
} SegmentTableEntry;

/**
 * @struct SegmentTable
 * @brief Collection of segments for a process.
 */
typedef struct {
    int process_id;
    int num_segments;
    SegmentTableEntry entries[MAX_SEGMENTS];
} SegmentTable;

/**
 * @struct SegmentTranslationResult
 * @brief Result of segment address translation.
 */
typedef struct {
    int segment_id;
    int offset;
    int base_address;
    int limit;
    int physical_address;
    bool is_valid_translation;
    bool is_segmentation_fault;
    char error_message[128];
} SegmentTranslationResult;

/* Initialize segment table */
void init_segment_table(SegmentTable *st, int process_id);

/* Add a segment descriptor */
bool add_segment(SegmentTable *st, int segment_id, const char *name, int base_addr, int limit);

/* Perform segment address translation */
SegmentTranslationResult translate_segment_address(const SegmentTable *st, int seg_id, int offset);

/* Display formatted Segment Table */
void display_segment_table(const SegmentTable *st);

/* Interactive segmentation simulator runner */
void run_segmentation_demo(SegmentTable *st);

#endif /* SEGMENTATION_H */
