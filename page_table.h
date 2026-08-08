/**
 * @file page_table.h
 * @brief Page Table Data Structures and Operations
 * 
 * Operating System Concept:
 * A Page Table is a data structure maintained by the OS kernel for each process.
 * It maps virtual/logical page numbers (VPN) to physical frame numbers (PFN).
 * The Present/Valid bit indicates whether the page currently resides in physical RAM.
 */

#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <stdbool.h>

#define MAX_PAGES 128

/**
 * @struct PageTableEntry
 * @brief Single entry in a process's page table.
 */
typedef struct {
    int page_number;      /* Virtual Page Number (VPN) */
    int frame_number;     /* Physical Frame Number (PFN) mapped to (-1 if not in RAM) */
    bool valid;           /* Present/Valid Bit: 1 if in memory, 0 if page fault */
    bool dirty;           /* Dirty Bit: 1 if modified, 0 if clean */
    int last_accessed;    /* Last access logical clock timestamp (for LRU) */
} PageTableEntry;

/**
 * @struct PageTable
 * @brief Per-process Page Table mapping pages to frames.
 */
typedef struct {
    int process_id;                  /* Process identifier */
    int num_pages;                   /* Total pages belonging to this process */
    PageTableEntry entries[MAX_PAGES];/* Array of page table entries */
} PageTable;

/* Initialize page table for a process */
void init_page_table(PageTable *pt, int process_id, int num_pages);

/* Map a page number to a frame number */
void map_page(PageTable *pt, int page_num, int frame_num);

/* Unmap a page (page evicted or freed) */
void unmap_page(PageTable *pt, int page_num);

/* Look up frame mapped to page number. Returns -1 if page fault / invalid */
int lookup_page(const PageTable *pt, int page_num, bool *is_valid);

/* Display formatted ASCII Page Table */
void display_page_table(const PageTable *pt);

#endif /* PAGE_TABLE_H */
