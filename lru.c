/**
 * @file lru.c
 * @brief Implementation of Least Recently Used (LRU) Page Replacement Algorithm
 */

#include <stdio.h>
#include <string.h>
#include "lru.h"
#include "utils.h"

/**
 * Runs LRU Page Replacement Algorithm.
 */
void run_lru_replacement(int frames_count, const int *ref_string, int ref_len, ReplacementResult *result) {
    memset(result, 0, sizeof(ReplacementResult));
    snprintf(result->algo_name, sizeof(result->algo_name), "LRU");
    result->num_frames = frames_count > MAX_SIM_FRAMES ? MAX_SIM_FRAMES : frames_count;
    result->ref_string_len = ref_len > MAX_REF_STRING ? MAX_REF_STRING : ref_len;

    for (int i = 0; i < result->ref_string_len; i++) {
        result->ref_string[i] = ref_string[i];
    }

    int current_frames[MAX_SIM_FRAMES];
    int last_used_time[MAX_SIM_FRAMES];

    for (int f = 0; f < result->num_frames; f++) {
        current_frames[f] = -1;
        last_used_time[f] = -1;
    }

    int fault_count = 0;
    int hit_count = 0;

    for (int i = 0; i < result->ref_string_len; i++) {
        int page = ref_string[i];
        int logical_time = i + 1;
        int hit_frame_slot = -1;

        /* Search if page is already loaded in physical frame */
        for (int f = 0; f < result->num_frames; f++) {
            if (current_frames[f] == page) {
                hit_frame_slot = f;
                break;
            }
        }

        StepTrace *step = &result->steps[i];
        step->step = i + 1;
        step->page_requested = page;
        step->num_frames = result->num_frames;
        step->evicted_page = -1;

        if (hit_frame_slot != -1) {
            /* Page Hit: update access time */
            hit_count++;
            step->is_page_fault = false;
            last_used_time[hit_frame_slot] = logical_time;
        } else {
            /* Page Fault */
            fault_count++;
            step->is_page_fault = true;

            /* Look for an unallocated frame slot */
            int empty_slot = -1;
            for (int f = 0; f < result->num_frames; f++) {
                if (current_frames[f] == -1) {
                    empty_slot = f;
                    break;
                }
            }

            if (empty_slot != -1) {
                current_frames[empty_slot] = page;
                last_used_time[empty_slot] = logical_time;
            } else {
                /* Find frame with minimum last_used_time (Least Recently Used) */
                int lru_slot = 0;
                int min_time = last_used_time[0];

                for (int f = 1; f < result->num_frames; f++) {
                    if (last_used_time[f] < min_time) {
                        min_time = last_used_time[f];
                        lru_slot = f;
                    }
                }

                step->evicted_page = current_frames[lru_slot];
                current_frames[lru_slot] = page;
                last_used_time[lru_slot] = logical_time;
            }
        }

        /* Record snapshot of physical frame contents */
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
