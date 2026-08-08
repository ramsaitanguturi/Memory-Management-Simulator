/**
 * @file comparison.h
 * @brief Performance Comparison Engine for Page Replacement Algorithms
 * 
 * Operating System Concept:
 * Evaluates performance efficiency across FIFO, LRU, and Optimal page replacement
 * using identical frame allocations and page reference strings.
 */

#ifndef COMPARISON_H
#define COMPARISON_H

#include "fifo.h"
#include "lru.h"
#include "optimal.h"

/**
 * Executes FIFO, LRU, and Optimal algorithms on reference string and prints side-by-side comparison table.
 */
void run_algorithm_comparison(int frames_count, const int *ref_string, int ref_len);

#endif /* COMPARISON_H */
