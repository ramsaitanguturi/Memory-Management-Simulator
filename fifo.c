/**
 * @file fifo.c
 * @brief Implementation of FIFO Page Replacement Algorithm & Step Analysis
 */

#include <stdio.h>
#include <string.h>
#include "fifo.h"
#include "utils.h"

/**
 * Runs FIFO Page Replacement.
 */
void run_fifo_replacement(int frames_count, const int *ref_string, int ref_len, ReplacementResult *result) {
    memset(result, 0, sizeof(ReplacementResult));
    snprintf(result->algo_name, sizeof(result->algo_name), "FIFO");
    result->num_frames = frames_count > MAX_SIM_FRAMES ? MAX_SIM_FRAMES : frames_count;
    result->ref_string_len = ref_len > MAX_REF_STRING ? MAX_REF_STRING : ref_len;

    for (int i = 0; i < result->ref_string_len; i++) {
        result->ref_string[i] = ref_string[i];
    }

    int current_frames[MAX_SIM_FRAMES];
    for (int f = 0; f < result->num_frames; f++) {
        current_frames[f] = -1;
    }

    int fifo_queue_pointer = 0;
    int fault_count = 0;
    int hit_count = 0;

    for (int i = 0; i < result->ref_string_len; i++) {
        int page = ref_string[i];
        bool found = false;

        /* Check if page is already loaded in physical frame (Page Hit) */
        for (int f = 0; f < result->num_frames; f++) {
            if (current_frames[f] == page) {
                found = true;
                break;
            }
        }

        StepTrace *step = &result->steps[i];
        step->step = i + 1;
        step->page_requested = page;
        step->num_frames = result->num_frames;
        step->evicted_page = -1;

        if (found) {
            /* Page Hit */
            hit_count++;
            step->is_page_fault = false;
        } else {
            /* Page Fault */
            fault_count++;
            step->is_page_fault = true;

            /* Find empty frame slot if available */
            int empty_slot = -1;
            for (int f = 0; f < result->num_frames; f++) {
                if (current_frames[f] == -1) {
                    empty_slot = f;
                    break;
                }
            }

            if (empty_slot != -1) {
                /* Place into empty frame */
                current_frames[empty_slot] = page;
            } else {
                /* Evict oldest page according to FIFO queue pointer */
                step->evicted_page = current_frames[fifo_queue_pointer];
                current_frames[fifo_queue_pointer] = page;
                fifo_queue_pointer = (fifo_queue_pointer + 1) % result->num_frames;
            }
        }

        /* Copy current frame snapshot */
        for (int f = 0; f < result->num_frames; f++) {
            step->frames[f] = current_frames[f];
        }
    }

    result->step_count = result->ref_string_len;
    result->total_page_faults = fault_count;
    result->total_hits = hit_count;
    result->hit_ratio = (double)hit_count / result->ref_string_len * 100.0;
    result->fault_ratio = (double)fault_count / result->ref_string_len * 100.0;
}

/**
 * Displays step-by-step trace for page replacement execution.
 */
void display_replacement_trace(const ReplacementResult *result) {
    char header_buf[128];
    snprintf(header_buf, sizeof(header_buf), "%s PAGE REPLACEMENT STEP-BY-STEP TRACE", result->algo_name);
    print_header(header_buf);

    printf(" Reference String: ");
    for (int i = 0; i < result->ref_string_len; i++) {
        printf("%d ", result->ref_string[i]);
    }
    printf("\n Total Frames    : %d\n", result->num_frames);
    print_separator();

    printf("+------+------+---------------------+------------+\n");
    printf("| Step | Page | Physical Frames     | Page Fault |\n");
    printf("+------+------+---------------------+------------+\n");

    for (int i = 0; i < result->step_count; i++) {
        const StepTrace *st = &result->steps[i];
        printf("| %-4d | %-4d | [ ", st->step, st->page_requested);

        for (int f = 0; f < st->num_frames; f++) {
            if (st->frames[f] != -1) {
                printf("%d ", st->frames[f]);
            } else {
                printf("- ");
            }
        }
        printf("] ");

        /* Padding space for frames alignment */
        int printed_chars = st->num_frames * 2 + 4;
        for (int p = printed_chars; p < 20; p++) {
            printf(" ");
        }

        if (st->is_page_fault) {
            if (st->evicted_page != -1) {
                printf("| Yes (Evict %d)|\n", st->evicted_page);
            } else {
                printf("| Yes        |\n");
            }
        } else {
            printf("| No (Hit)   |\n");
        }
    }
    printf("+------+------+---------------------+------------+\n\n");

    printf(" Performance Summary for %s:\n", result->algo_name);
    printf("   Total References : %d\n", result->ref_string_len);
    printf("   Total Page Faults: %d\n", result->total_page_faults);
    printf("   Total Page Hits  : %d\n", result->total_hits);
    printf("   Hit Ratio        : %.2f%%\n", result->hit_ratio);
    printf("   Fault Ratio      : %.2f%%\n", result->fault_ratio);
    print_separator();
}
