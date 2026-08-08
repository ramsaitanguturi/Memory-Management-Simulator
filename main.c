/**
 * @file main.c
 * @brief Memory Management Simulator - Main Interactive Console Entry Point
 * 
 * Operating System Course Project: Memory Management Module Simulator
 * Concepts Implemented:
 *  - Paging & MMU Address Translation
 *  - Segmentation Table & Boundary Limit Checking (Segmentation Faults)
 *  - Page Replacement Algorithms (FIFO, LRU, Optimal)
 *  - Page Fault & Hit Analysis
 *  - Performance Comparison Matrix
 *  - Physical Memory Frame ASCII Visualizer
 *  - Configuration File Parsing & Simulation Report Exporter
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "memory.h"
#include "page_table.h"
#include "paging.h"
#include "segmentation.h"
#include "fifo.h"
#include "lru.h"
#include "optimal.h"
#include "comparison.h"
#include "gantt_memory.h"
#include "utils.h"

int main(void) {
    /* State variables */
    MemoryConfig mem_config;
    PageTable main_pt;
    SegmentTable main_st;

    int ref_string[MAX_REF_STRING] = {7, 0, 1, 2, 0, 3, 0, 4};
    int ref_string_len = 8;
    int sim_frames = 3;

    /* Initialize default simulator parameters */
    init_memory_config(&mem_config, 1024, 64, 3);

    /* Initialize process P1 Page Table matching textbook example */
    init_page_table(&main_pt, 1, 4);
    map_page(&main_pt, 0, 5);
    map_page(&main_pt, 1, 8);
    map_page(&main_pt, 2, 1);
    map_page(&main_pt, 3, 10);

    /* Mark frames in memory */
    allocate_frame(&mem_config, 5, 1, 0);
    allocate_frame(&mem_config, 8, 1, 1);
    allocate_frame(&mem_config, 1, 1, 2);
    allocate_frame(&mem_config, 10, 1, 3);

    /* Initialize process P1 Segment Table */
    init_segment_table(&main_st, 1);
    add_segment(&main_st, 0, "Code Segment", 1000, 500);
    add_segment(&main_st, 1, "Data Segment", 2000, 300);
    add_segment(&main_st, 2, "Stack Segment", 3000, 700);

    int choice = -1;

    while (choice != 0) {
        print_header("========= Memory Management Simulator =========");
        printf(" 1. Configure Memory\n");
        printf(" 2. Paging Simulation & Address Translation\n");
        printf(" 3. Segmentation Simulation & Address Translation\n");
        printf(" 4. FIFO Page Replacement Algorithm\n");
        printf(" 5. LRU Page Replacement Algorithm\n");
        printf(" 6. Optimal Page Replacement Algorithm\n");
        printf(" 7. Compare Page Replacement Algorithms\n");
        printf(" 8. ASCII Memory Frame Visualization\n");
        printf(" 9. Load Configuration from File (sample_input.txt)\n");
        printf(" 10. Save Output Report (memory_report.txt)\n");
        printf(" 0. Exit Simulator\n");
        print_separator();
        printf(" Select Option (0-10): ");

        choice = read_int_input();

        switch (choice) {
            case 1: {
                print_header("MODULE 1: MEMORY CONFIGURATION SETUP");
                printf("Enter Total Physical Memory in KB (e.g. 1024): ");
                int total_mem = read_int_input();

                printf("Enter Page / Frame Size in KB (e.g. 64): ");
                int page_sz = read_int_input();

                printf("Enter Number of Processes (e.g. 3): ");
                int n_procs = read_int_input();

                if (validate_memory_params(total_mem, page_sz, n_procs)) {
                    init_memory_config(&mem_config, total_mem, page_sz, n_procs);
                    print_success("Physical Memory Configured Successfully!");
                }
                display_memory_config(&mem_config);
                break;
            }

            case 2:
                run_paging_demo(&mem_config, &main_pt);
                break;

            case 3:
                run_segmentation_demo(&main_st);
                break;

            case 4: {
                print_header("MODULE 4: FIFO PAGE REPLACEMENT");
                printf("Current Reference String: ");
                for (int i = 0; i < ref_string_len; i++) printf("%d ", ref_string[i]);
                printf("\nEnter Replacement Frame Count (e.g. %d): ", sim_frames);
                int input_f = read_int_input();
                if (input_f > 0) sim_frames = input_f;

                ReplacementResult res;
                run_fifo_replacement(sim_frames, ref_string, ref_string_len, &res);
                display_replacement_trace(&res);
                break;
            }

            case 5: {
                print_header("MODULE 4: LRU PAGE REPLACEMENT");
                printf("Current Reference String: ");
                for (int i = 0; i < ref_string_len; i++) printf("%d ", ref_string[i]);
                printf("\nEnter Replacement Frame Count (e.g. %d): ", sim_frames);
                int input_f = read_int_input();
                if (input_f > 0) sim_frames = input_f;

                ReplacementResult res;
                run_lru_replacement(sim_frames, ref_string, ref_string_len, &res);
                display_replacement_trace(&res);
                break;
            }

            case 6: {
                print_header("MODULE 4: OPTIMAL PAGE REPLACEMENT");
                printf("Current Reference String: ");
                for (int i = 0; i < ref_string_len; i++) printf("%d ", ref_string[i]);
                printf("\nEnter Replacement Frame Count (e.g. %d): ", sim_frames);
                int input_f = read_int_input();
                if (input_f > 0) sim_frames = input_f;

                ReplacementResult res;
                run_optimal_replacement(sim_frames, ref_string, ref_string_len, &res);
                display_replacement_trace(&res);
                break;
            }

            case 7:
                print_header("MODULE 6: PERFORMANCE COMPARISON");
                printf("Enter Replacement Frame Count (e.g. %d): ", sim_frames);
                int comp_f = read_int_input();
                if (comp_f > 0) sim_frames = comp_f;

                printf("Would you like to enter a custom reference string? (y/n): ");
                char custom_resp[16];
                read_string_input(custom_resp, sizeof(custom_resp));
                if (custom_resp[0] == 'y' || custom_resp[0] == 'Y') {
                    printf("Enter space-separated reference string (e.g. 7 0 1 2 0 3 0 4): ");
                    char str_buf[256];
                    read_string_input(str_buf, sizeof(str_buf));
                    int parsed = parse_reference_string(str_buf, ref_string, MAX_REF_STRING);
                    if (parsed > 0) {
                        ref_string_len = parsed;
                        print_success("Reference string updated!");
                    }
                }
                run_algorithm_comparison(sim_frames, ref_string, ref_string_len);
                break;

            case 8:
                display_ascii_memory_frames(&mem_config);
                display_memory_map_boxes(&mem_config);
                break;

            case 9:
                load_config_file("sample_input.txt", &mem_config, &main_pt, &main_st, ref_string, &ref_string_len);
                break;

            case 10:
                export_simulation_report("memory_report.txt", &mem_config, &main_pt, &main_st, sim_frames, ref_string, ref_string_len);
                break;

            case 0:
                print_success("Exiting Memory Management Simulator. Goodbye!");
                break;

            default:
                print_error("Invalid selection. Please choose an option between 0 and 10.");
                break;
        }
    }

    return 0;
}
