/**
 * @file lru.h
 * @brief Least Recently Used (LRU) Page Replacement Header
 * 
 * Operating System Concept:
 * LRU page replacement evicts the page that has not been accessed for the longest period of time.
 * It uses past reference history as an approximation of future behavior (locality of reference principle).
 */

#ifndef LRU_H
#define LRU_H

#include "fifo.h" /* Shares ReplacementResult and StepTrace definitions */

/* Executes LRU page replacement algorithm */
void run_lru_replacement(int frames_count, const int *ref_string, int ref_len, ReplacementResult *result);

#endif /* LRU_H */
