/**
 * @file utils.c
 * @brief Implementation of terminal formatting, input sanitization, file I/O, and report export
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"
#include "lru.h"
#include "optimal.h"

void print_header(const char *title) {
    printf("\n%s================================================================================%s\n", COLOR_CYAN, COLOR_RESET);
    printf(" %s%s%s\n", COLOR_BOLD, title, COLOR_RESET);
    printf("%s================================================================================%s\n", COLOR_CYAN, COLOR_RESET);
}

void print_separator(void) {
    printf("%s--------------------------------------------------------------------------------%s\n", COLOR_CYAN, COLOR_RESET);
}

void print_success(const char *msg) {
    printf("%s[SUCCESS] %s%s\n", COLOR_GREEN, msg, COLOR_RESET);
}

void print_error(const char *msg) {
    printf("%s[ERROR] %s%s\n", COLOR_RED, msg, COLOR_RESET);
}

void print_warning(const char *msg) {
    printf("%s[WARNING] %s%s\n", COLOR_YELLOW, msg, COLOR_RESET);
}

int read_int_input(void) {
    char buffer[64];
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    return atoi(buffer);
}

void read_string_input(char *buffer, int max_len) {
    if (fgets(buffer, max_len, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

/**
 * Parses space-separated integers from string into an integer array.
 */
int parse_reference_string(const char *str_input, int *output_array, int max_size) {
    int count = 0;
    const char *ptr = str_input;

    while (*ptr != '\0' && count < max_size) {
        while (*ptr != '\0' && (isspace((unsigned char)*ptr) || *ptr == ',')) {
            ptr++;
        }
        if (*ptr == '\0') break;

        if (isdigit((unsigned char)*ptr) || (*ptr == '-' && isdigit((unsigned char)*(ptr + 1)))) {
            output_array[count++] = atoi(ptr);
            if (*ptr == '-') ptr++;
            while (*ptr != '\0' && isdigit((unsigned char)*ptr)) {
                ptr++;
            }
        } else {
            ptr++;
        }
    }

    return count;
}

/**
 * Loads memory configuration and reference string from text file (sample_input.txt).
 */
bool load_config_file(const char *filename, MemoryConfig *config, PageTable *pt, SegmentTable *st, int *ref_string, int *ref_len) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        print_error("Failed to open input file.");
        return false;
    }

    int mem_size = 1024;
    int page_size = 64;
    int num_procs = 3;
    char line[256];
    int parsed_ref_len = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        /* Strip comments or line endings */
        char *hash = strchr(line, '#');
        if (hash) *hash = '\0';

        if (strstr(line, "Memory Size:") || strstr(line, "Memory Size")) {
            char *colon = strchr(line, ':');
            if (colon) mem_size = atoi(colon + 1);
        } else if (strstr(line, "Page Size:") || strstr(line, "Page Size")) {
            char *colon = strchr(line, ':');
            if (colon) page_size = atoi(colon + 1);
        } else if (strstr(line, "Number of Processes:") || strstr(line, "Number of Processes")) {
            char *colon = strchr(line, ':');
            if (colon) num_procs = atoi(colon + 1);
        } else if (strstr(line, "Reference String:") || strstr(line, "Reference String")) {
            char *colon = strchr(line, ':');
            if (colon) {
                parsed_ref_len = parse_reference_string(colon + 1, ref_string, MAX_REF_STRING);
            }
        }
    }

    fclose(fp);

    if (!validate_memory_params(mem_size, page_size, num_procs)) {
        return false;
    }

    init_memory_config(config, mem_size, page_size, num_procs);
    init_page_table(pt, 1, 4);

    /* Map initial default sample pages for process P1 */
    map_page(pt, 0, 5);
    map_page(pt, 1, 8);
    map_page(pt, 2, 1);
    map_page(pt, 3, 10);

    /* Allocate corresponding frames in physical memory struct */
    allocate_frame(config, 5, 1, 0);
    allocate_frame(config, 8, 1, 1);
    allocate_frame(config, 1, 1, 2);
    allocate_frame(config, 10, 1, 3);

    init_segment_table(st, 1);
    add_segment(st, 0, "Code Segment", 1000, 500);
    add_segment(st, 1, "Data Segment", 2000, 300);
    add_segment(st, 2, "Stack Segment", 3000, 700);

    if (parsed_ref_len > 0) {
        *ref_len = parsed_ref_len;
    }

    print_success("Configuration successfully loaded from file!");
    return true;
}

/**
 * Saves complete simulation report to memory_report.txt.
 */
bool export_simulation_report(const char *filename, const MemoryConfig *config, const PageTable *pt, const SegmentTable *st, int frames_count, const int *ref_string, int ref_len) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        print_error("Unable to create report file.");
        return false;
    }

    fprintf(fp, "================================================================================\n");
    fprintf(fp, "                    OPERATING SYSTEM MEMORY MANAGEMENT REPORT\n");
    fprintf(fp, "================================================================================\n\n");

    /* Section 1: Memory Configuration */
    fprintf(fp, "[MODULE 1: MEMORY CONFIGURATION]\n");
    fprintf(fp, "Total Physical Memory : %d KB\n", config->total_memory_kb);
    fprintf(fp, "Page / Frame Size     : %d KB\n", config->page_size_kb);
    fprintf(fp, "Total Frames          : %d\n", config->total_frames);
    fprintf(fp, "Free Frames           : %d\n", config->free_frames);
    fprintf(fp, "Allocated Frames      : %d\n", config->total_frames - config->free_frames);
    fprintf(fp, "Number of Processes   : %d\n\n", config->num_processes);

    /* Section 2: Page Table */
    fprintf(fp, "[MODULE 2: PAGE TABLE FOR PROCESS P%d]\n", pt->process_id);
    fprintf(fp, "+-------+-------+---------+\n");
    fprintf(fp, "| Page  | Frame | Valid   |\n");
    fprintf(fp, "+-------+-------+---------+\n");
    for (int i = 0; i < pt->num_pages; i++) {
        if (pt->entries[i].valid) {
            fprintf(fp, "| %-5d | %-5d | Yes     |\n", pt->entries[i].page_number, pt->entries[i].frame_number);
        } else {
            fprintf(fp, "| %-5d | %-5s | No      |\n", pt->entries[i].page_number, "-");
        }
    }
    fprintf(fp, "+-------+-------+---------+\n\n");

    /* Section 3: Segment Table */
    fprintf(fp, "[MODULE 3: SEGMENT TABLE FOR PROCESS P%d]\n", st->process_id);
    fprintf(fp, "+---------+-----------------+--------------+--------------+\n");
    fprintf(fp, "| Segment | Name            | Base Address | Limit (Size) |\n");
    fprintf(fp, "+---------+-----------------+--------------+--------------+\n");
    for (int i = 0; i < st->num_segments; i++) {
        if (st->entries[i].valid) {
            fprintf(fp, "| %-7d | %-15s | %-12d | %-12d |\n",
                    st->entries[i].segment_id, st->entries[i].name, st->entries[i].base_address, st->entries[i].limit);
        }
    }
    fprintf(fp, "+---------+-----------------+--------------+--------------+\n\n");

    /* Section 4: Page Replacement Runs */
    ReplacementResult fifo_res, lru_res, opt_res;
    run_fifo_replacement(frames_count, ref_string, ref_len, &fifo_res);
    run_lru_replacement(frames_count, ref_string, ref_len, &lru_res);
    run_optimal_replacement(frames_count, ref_string, ref_len, &opt_res);

    fprintf(fp, "[MODULE 4 & 5: PAGE REPLACEMENT STEP TRACES]\n");
    fprintf(fp, "Reference String: ");
    for (int i = 0; i < ref_len; i++) {
        fprintf(fp, "%d ", ref_string[i]);
    }
    fprintf(fp, "\nAllocated Replacement Frames: %d\n\n", frames_count);

    const ReplacementResult *algos[3] = {&fifo_res, &lru_res, &opt_res};
    for (int a = 0; a < 3; a++) {
        const ReplacementResult *r = algos[a];
        fprintf(fp, "--- %s Page Replacement ---\n", r->algo_name);
        fprintf(fp, "+------+------+---------------------+------------+\n");
        fprintf(fp, "| Step | Page | Physical Frames     | Page Fault |\n");
        fprintf(fp, "+------+------+---------------------+------------+\n");
        for (int i = 0; i < r->step_count; i++) {
            const StepTrace *st_trace = &r->steps[i];
            fprintf(fp, "| %-4d | %-4d | [ ", st_trace->step, st_trace->page_requested);
            for (int f = 0; f < st_trace->num_frames; f++) {
                if (st_trace->frames[f] != -1) {
                    fprintf(fp, "%d ", st_trace->frames[f]);
                } else {
                    fprintf(fp, "- ");
                }
            }
            fprintf(fp, "] ");
            if (st_trace->is_page_fault) {
                if (st_trace->evicted_page != -1) {
                    fprintf(fp, "| Yes (Evict %d)|\n", st_trace->evicted_page);
                } else {
                    fprintf(fp, "| Yes        |\n");
                }
            } else {
                fprintf(fp, "| No (Hit)   |\n");
            }
        }
        fprintf(fp, "+------+------+---------------------+------------+\n");
        fprintf(fp, "Total Faults: %d | Total Hits: %d | Hit Ratio: %.2f%%\n\n",
                r->total_page_faults, r->total_hits, r->hit_ratio);
    }

    /* Section 5: Comparison */
    fprintf(fp, "[MODULE 6: ALGORITHM PERFORMANCE COMPARISON]\n");
    fprintf(fp, "+------------------+-------------+------------+---------------+-----------------+\n");
    fprintf(fp, "| Algorithm        | Page Faults | Page Hits  | Hit Ratio (%%) | Fault Ratio (%%) |\n");
    fprintf(fp, "+------------------+-------------+------------+---------------+-----------------+\n");
    fprintf(fp, "| %-16s | %-11d | %-10d | %-13.2f | %-15.2f |\n",
            fifo_res.algo_name, fifo_res.total_page_faults, fifo_res.total_hits, fifo_res.hit_ratio, fifo_res.fault_ratio);
    fprintf(fp, "| %-16s | %-11d | %-10d | %-13.2f | %-15.2f |\n",
            lru_res.algo_name, lru_res.total_page_faults, lru_res.total_hits, lru_res.hit_ratio, lru_res.fault_ratio);
    fprintf(fp, "| %-16s | %-11d | %-10d | %-13.2f | %-15.2f |\n",
            opt_res.algo_name, opt_res.total_page_faults, opt_res.total_hits, opt_res.hit_ratio, opt_res.fault_ratio);
    fprintf(fp, "+------------------+-------------+------------+---------------+-----------------+\n\n");

    const ReplacementResult *best = &fifo_res;
    if (lru_res.total_page_faults < best->total_page_faults) best = &lru_res;
    if (opt_res.total_page_faults < best->total_page_faults) best = &opt_res;

    fprintf(fp, "SUMMARY CONCLUSION:\n");
    fprintf(fp, "Best Algorithm: %s with %d page faults (Hit Ratio: %.2f%%)\n",
            best->algo_name, best->total_page_faults, best->hit_ratio);
    fprintf(fp, "================================================================================\n");

    fclose(fp);
    print_success("Simulation report exported successfully to memory_report.txt");
    return true;
}
