/**
 * @file page_table.c
 * @brief Implementation of Page Table mapping and visual display
 */

#include <stdio.h>
#include <stdbool.h>
#include "page_table.h"
#include "utils.h"

/**
 * Initializes a page table for a process.
 */
void init_page_table(PageTable *pt, int process_id, int num_pages) {
    pt->process_id = process_id;
    pt->num_pages = num_pages > MAX_PAGES ? MAX_PAGES : num_pages;

    for (int i = 0; i < pt->num_pages; i++) {
        pt->entries[i].page_number = i;
        pt->entries[i].frame_number = -1;
        pt->entries[i].valid = false;
        pt->entries[i].dirty = false;
        pt->entries[i].last_accessed = 0;
    }
}

/**
 * Maps a logical page to a physical frame.
 */
void map_page(PageTable *pt, int page_num, int frame_num) {
    if (page_num >= 0 && page_num < pt->num_pages) {
        pt->entries[page_num].frame_number = frame_num;
        pt->entries[page_num].valid = true;
    }
}

/**
 * Unmaps a page from physical memory (invalidates entry).
 */
void unmap_page(PageTable *pt, int page_num) {
    if (page_num >= 0 && page_num < pt->num_pages) {
        pt->entries[page_num].frame_number = -1;
        pt->entries[page_num].valid = false;
    }
}

/**
 * Searches the page table for a given page number.
 */
int lookup_page(const PageTable *pt, int page_num, bool *is_valid) {
    if (page_num < 0 || page_num >= pt->num_pages) {
        if (is_valid) *is_valid = false;
        return -1;
    }

    if (is_valid) {
        *is_valid = pt->entries[page_num].valid;
    }

    return pt->entries[page_num].frame_number;
}

/**
 * Formats and prints the Page Table matching standard OS course textbook outputs.
 */
void display_page_table(const PageTable *pt) {
    char header_buf[64];
    snprintf(header_buf, sizeof(header_buf), "PAGE TABLE FOR PROCESS P%d", pt->process_id);
    print_header(header_buf);

    printf("+-------+-------+---------+\n");
    printf("| Page  | Frame | Valid   |\n");
    printf("+-------+-------+---------+\n");

    for (int i = 0; i < pt->num_pages; i++) {
        if (pt->entries[i].valid) {
            printf("| %-5d | %-5d | Yes     |\n", pt->entries[i].page_number, pt->entries[i].frame_number);
        } else {
            printf("| %-5d | %-5s | No      |\n", pt->entries[i].page_number, "-");
        }
    }
    printf("+-------+-------+---------+\n\n");
}
