/**
 * @file comparison.c
 * @brief Implementation of side-by-side performance comparison for Page Replacement Algorithms
 */

#include <stdio.h>
#include "comparison.h"
#include "utils.h"

void run_algorithm_comparison(int frames_count, const int *ref_string, int ref_len) {
    if (ref_len <= 0 || frames_count <= 0) {
        print_error("Cannot perform comparison on empty reference string or zero frames.");
        return;
    }

    ReplacementResult fifo_res;
    ReplacementResult lru_res;
    ReplacementResult opt_res;

    run_fifo_replacement(frames_count, ref_string, ref_len, &fifo_res);
    run_lru_replacement(frames_count, ref_string, ref_len, &lru_res);
    run_optimal_replacement(frames_count, ref_string, ref_len, &opt_res);

    print_header("MODULE 6: PAGE REPLACEMENT ALGORITHMS COMPARISON");

    printf(" Test Parameters:\n");
    printf("   Physical Frames Allocated : %d\n", frames_count);
    printf("   Reference String Length   : %d\n", ref_len);
    printf("   Reference String          : ");
    for (int i = 0; i < ref_len; i++) {
        printf("%d ", ref_string[i]);
    }
    printf("\n");
    print_separator();

    printf("+------------------+-------------+------------+---------------+-----------------+\n");
    printf("| Algorithm        | Page Faults | Page Hits  | Hit Ratio (%%) | Fault Ratio (%%) |\n");
    printf("+------------------+-------------+------------+---------------+-----------------+\n");
    printf("| %-16s | %-11d | %-10d | %-13.2f | %-15.2f |\n",
           fifo_res.algo_name, fifo_res.total_page_faults, fifo_res.total_hits, fifo_res.hit_ratio, fifo_res.fault_ratio);
    printf("| %-16s | %-11d | %-10d | %-13.2f | %-15.2f |\n",
           lru_res.algo_name, lru_res.total_page_faults, lru_res.total_hits, lru_res.hit_ratio, lru_res.fault_ratio);
    printf("| %-16s | %-11d | %-10d | %-13.2f | %-15.2f |\n",
           opt_res.algo_name, opt_res.total_page_faults, opt_res.total_hits, opt_res.hit_ratio, opt_res.fault_ratio);
    printf("+------------------+-------------+------------+---------------+-----------------+\n\n");

    /* Determine best algorithm */
    const ReplacementResult *best = &fifo_res;
    if (lru_res.total_page_faults < best->total_page_faults) {
        best = &lru_res;
    }
    if (opt_res.total_page_faults < best->total_page_faults) {
        best = &opt_res;
    }

    print_success("PERFORMANCE ANALYSIS CONCLUSION:");
    printf("   Best Performing Algorithm : %s\n", best->algo_name);
    printf("   Lowest Page Fault Count   : %d faults\n", best->total_page_faults);
    printf("   Highest Hit Ratio         : %.2f%%\n", best->hit_ratio);
    print_separator();
}
