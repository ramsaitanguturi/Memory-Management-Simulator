/**
 * @file paging.h
 * @brief Paging Simulation & Logical-to-Physical Address Translation
 * 
 * Operating System Concept:
 * Paging eliminates contiguous physical memory allocation requirements by dividing
 * virtual memory into equal pages and physical RAM into frames.
 * The CPU's Memory Management Unit (MMU) translates a logical address (Page, Offset)
 * into a physical address (Frame * Page_Size + Offset).
 */

#ifndef PAGING_H
#define PAGING_H

#include <stdbool.h>
#include "memory.h"
#include "page_table.h"

/**
 * @struct AddressTranslationResult
 * @brief Stores details of an address translation step.
 */
typedef struct {
    int logical_address;
    int page_number;
    int offset;
    int frame_number;
    int physical_address;
    bool is_valid_translation;
    char error_message[128];
} AddressTranslationResult;

/* Perform address translation given Page Number & Offset */
AddressTranslationResult translate_page_offset(const PageTable *pt, const MemoryConfig *config, int page_num, int offset);

/* Perform address translation given a single raw Logical Address */
AddressTranslationResult translate_logical_address(const PageTable *pt, const MemoryConfig *config, int logical_addr);

/* Interactive Paging Simulation Runner */
void run_paging_demo(MemoryConfig *config, PageTable *pt);

#endif /* PAGING_H */
