/**
 * @file optimal.c
 * @brief Implementation of Optimal (Belady's Min) Page Replacement Algorithm
 */

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "optimal.h"
#include "utils.h"

/**
 * Finds the index of the frame whose page will be referenced furthest in the future.
 */
static int find_optimal_victim_slot(const int *frames, int num_frames, const int *ref_string, int ref_len, int current_idx) {
    int victim_slot = -1;
    int furthest_use = -1;

    for (int f = 0; f < num_frames; f++) {
        int page = frames[f];
        int next_use = INT_MAX;

        /* Scan future reference string to find next reference of page */
        for (int k = current_idx + 1; k < ref_len; k++) {
            if (ref_string[k] == page) {
                next_use = k;
                break;
            }
        }

        /* If page is never referenced again, it is the ideal victim */
        if (next_use == INT_MAX) {
            return f;
        }

        if (next_use > furthest_use) {
            furthest_use = next_use;
            victim_slot = f;
        }
    }

    return (victim_slot != -1) ? victim_slot : 0;
}

/**
 * Runs Optimal Page Replacement Algorithm.
 */
void run_optimal_replacement(int frames_count, const int *ref_string, int ref_len, ReplacementResult *result) {
    memset(result, 0, sizeof(ReplacementResult));
    snprintf(result->algo_name, sizeof(result->algo_name), "Optimal");
    result->num_frames = frames_count > MAX_SIM_FRAMES ? MAX_SIM_FRAMES : frames_count;
    result->ref_string_len = ref_len > MAX_REF_STRING ? MAX_REF_STRING : ref_len;

    for (int i = 0; i < result->ref_string_len; i++) {
        result->ref_string[i] = ref_string[i];
    }

    int current_frames[MAX_SIM_FRAMES];
    for (int f = 0; f < result->num_frames; f++) {
        current_frames[f] = -1;
    }

    int fault_count = 0;
    int hit_count = 0;

    for (int i = 0; i < result->ref_string_len; i++) {
        int page = ref_string[i];
        bool found = false;

        /* Check if page is already loaded in frame */
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
            hit_count++;
            step->is_page_fault = false;
        } else {
            fault_count++;
            step->is_page_fault = true;

            /* Search for empty slot */
            int empty_slot = -1;
            for (int f = 0; f < result->num_frames; f++) {
                if (current_frames[f] == -1) {
                    empty_slot = f;
                    break;
                }
            }

            if (empty_slot != -1) {
                current_frames[empty_slot] = page;
            } else {
                /* Evict page that will not be used for longest time in future */
                int victim_slot = find_optimal_victim_slot(current_frames, result->num_frames, ref_string, result->ref_string_len, i);
                step->evicted_page = current_frames[victim_slot];
                current_frames[victim_slot] = page;
            }
        }

        /* Snapshot frame contents */
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
