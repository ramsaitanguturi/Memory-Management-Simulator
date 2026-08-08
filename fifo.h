/**
 * @file fifo.h
 * @brief First-In, First-Out (FIFO) Page Replacement Algorithm Header
 * 
 * Operating System Concept:
 * The FIFO page replacement algorithm maintains a queue of all pages currently in memory.
 * When a page fault occurs and memory is full, the OS evicts the page at the head of the queue
 * (i.e. the oldest page loaded into physical memory first).
 */

#ifndef FIFO_H
#define FIFO_H

#include <stdbool.h>

#define MAX_REF_STRING 100
#define MAX_SIM_FRAMES 32

/**
 * @struct StepTrace
 * @brief Represents state of physical memory frames at each step of reference string execution.
 */
typedef struct {
    int step;                   /* Step index (1, 2, 3, ...) */
    int page_requested;         /* Page number referenced in this step */
    int frames[MAX_SIM_FRAMES]; /* Snapshot of frame contents (-1 = empty) */
    int num_frames;             /* Allocated frame count */
    bool is_page_fault;         /* True if page fault occurred, False if page hit */
    int evicted_page;           /* Page number evicted (-1 if none) */
} StepTrace;

/**
 * @struct ReplacementResult
 * @brief Summary and full trace of a page replacement algorithm run.
 */
typedef struct {
    char algo_name[32];               /* Algorithm Name ("FIFO", "LRU", "Optimal") */
    int num_frames;                   /* Total available frames */
    int ref_string_len;               /* Number of page references */
    int ref_string[MAX_REF_STRING];   /* Complete reference string */
    int total_page_faults;            /* Count of page faults */
    int total_hits;                   /* Count of page hits */
    double hit_ratio;                 /* Percentage hit ratio */
    double fault_ratio;               /* Percentage fault ratio */
    int step_count;                   /* Total trace steps recorded */
    StepTrace steps[MAX_REF_STRING];  /* Step-by-step array */
} ReplacementResult;

/* Executes FIFO page replacement algorithm */
void run_fifo_replacement(int frames_count, const int *ref_string, int ref_len, ReplacementResult *result);

/* Displays step-by-step trace and page fault analysis table */
void display_replacement_trace(const ReplacementResult *result);

#endif /* FIFO_H */
