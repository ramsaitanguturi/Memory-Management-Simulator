/**
 * @file segmentation.c
 * @brief Implementation of Segmentation simulation, segment limit checking, and trap generation
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "segmentation.h"
#include "utils.h"

/**
 * Initializes a empty segment table for a process.
 */
void init_segment_table(SegmentTable *st, int process_id) {
    st->process_id = process_id;
    st->num_segments = 0;
    for (int i = 0; i < MAX_SEGMENTS; i++) {
        st->entries[i].segment_id = i;
        st->entries[i].name[0] = '\0';
        st->entries[i].base_address = 0;
        st->entries[i].limit = 0;
        st->entries[i].valid = false;
    }
}

/**
 * Adds a new segment entry to the segment table.
 */
bool add_segment(SegmentTable *st, int segment_id, const char *name, int base_addr, int limit) {
    if (segment_id < 0 || segment_id >= MAX_SEGMENTS) {
        return false;
    }

    st->entries[segment_id].segment_id = segment_id;
    snprintf(st->entries[segment_id].name, sizeof(st->entries[segment_id].name), "%s", name ? name : "Segment");
    st->entries[segment_id].base_address = base_addr;
    st->entries[segment_id].limit = limit;
    st->entries[segment_id].valid = true;

    if (segment_id >= st->num_segments) {
        st->num_segments = segment_id + 1;
    }
    return true;
}

/**
 * Translates (Segment Number, Offset) to Physical Address with boundary limit checking.
 */
SegmentTranslationResult translate_segment_address(const SegmentTable *st, int seg_id, int offset) {
    SegmentTranslationResult res;
    memset(&res, 0, sizeof(res));

    res.segment_id = seg_id;
    res.offset = offset;

    if (seg_id < 0 || seg_id >= st->num_segments || !st->entries[seg_id].valid) {
        res.is_valid_translation = false;
        res.is_segmentation_fault = false;
        snprintf(res.error_message, sizeof(res.error_message),
                 "Invalid Segment Number (%d). Segment does not exist in Segment Table.", seg_id);
        return res;
    }

    res.base_address = st->entries[seg_id].base_address;
    res.limit = st->entries[seg_id].limit;

    if (offset < 0) {
        res.is_valid_translation = false;
        res.is_segmentation_fault = false;
        snprintf(res.error_message, sizeof(res.error_message), "Offset cannot be negative.");
        return res;
    }

    /* Hardware Boundary Check: Offset < Limit */
    if (offset >= res.limit) {
        res.is_valid_translation = false;
        res.is_segmentation_fault = true;
        snprintf(res.error_message, sizeof(res.error_message),
                 "SEGMENTATION FAULT! Offset (%d) >= Segment Limit (%d). OS traps process P%d!",
                 offset, res.limit, st->process_id);
        return res;
    }

    res.physical_address = res.base_address + offset;
    res.is_valid_translation = true;
    res.is_segmentation_fault = false;
    return res;
}

/**
 * Displays formatted Segment Table.
 */
void display_segment_table(const SegmentTable *st) {
    char title_buf[64];
    snprintf(title_buf, sizeof(title_buf), "SEGMENT TABLE FOR PROCESS P%d", st->process_id);
    print_header(title_buf);

    printf("+---------+-----------------+--------------+--------------+\n");
    printf("| Segment | Name            | Base Address | Limit (Size) |\n");
    printf("+---------+-----------------+--------------+--------------+\n");

    for (int i = 0; i < st->num_segments; i++) {
        if (st->entries[i].valid) {
            printf("| %-7d | %-15s | %-12d | %-12d |\n",
                   st->entries[i].segment_id,
                   st->entries[i].name,
                   st->entries[i].base_address,
                   st->entries[i].limit);
        }
    }
    printf("+---------+-----------------+--------------+--------------+\n\n");
}

/**
 * Interactive Segmentation Simulation Runner.
 */
void run_segmentation_demo(SegmentTable *st) {
    print_header("MODULE 3: SEGMENTATION SIMULATION & ADDRESS TRANSLATION");

    display_segment_table(st);

    printf("Enter Segment Number (0 - %d): ", st->num_segments - 1);
    int seg_id = read_int_input();

    printf("Enter Logical Offset: ");
    int offset = read_int_input();

    printf("\nPerforming Segment MMU Boundary Validation...\n");
    print_separator();

    SegmentTranslationResult res = translate_segment_address(st, seg_id, offset);

    printf(" Logical Segment Input:\n");
    printf("   Segment Number   : %d\n", seg_id);
    printf("   Offset           : %d\n", offset);
    print_separator();

    if (res.is_valid_translation) {
        print_success("VALID ADDRESS - TRANSLATION SUCCESSFUL!");
        printf("   Base Address     : %d\n", res.base_address);
        printf("   Limit Check      : Offset (%d) < Limit (%d) [PASSED]\n", res.offset, res.limit);
        printf("   Calculation      : Base Address (%d) + Offset (%d)\n", res.base_address, res.offset);
        printf("   PHYSICAL ADDRESS : %d\n", res.physical_address);
    } else if (res.is_segmentation_fault) {
        print_error("TRAP TO OPERATING SYSTEM GENERATED!");
        print_error(res.error_message);
        printf("   Base Address     : %d\n", res.base_address);
        printf("   Limit Check      : Offset (%d) >= Limit (%d) [VIOLATION]\n", res.offset, res.limit);
    } else {
        print_error(res.error_message);
    }
    print_separator();
}
