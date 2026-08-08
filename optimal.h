/**
 * @file optimal.h
 * @brief Optimal (Belady's Min) Page Replacement Header
 * 
 * Operating System Concept:
 * Optimal page replacement (OPT or Belady's Algorithm) has the lowest possible page fault rate.
 * It evicts the page that will not be referenced for the longest time in the future.
 * Note: OPT requires future knowledge of the reference string, so it is used as a theoretical benchmark.
 */

#ifndef OPTIMAL_H
#define OPTIMAL_H

#include "fifo.h" /* Shares ReplacementResult and StepTrace definitions */

/* Executes Optimal page replacement algorithm */
void run_optimal_replacement(int frames_count, const int *ref_string, int ref_len, ReplacementResult *result);

#endif /* OPTIMAL_H */
