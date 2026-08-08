/**
 * @file paging.c
 * @brief Implementation of Paging Simulation & Address Translation
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "paging.h"
#include "utils.h"

/**
 * Translates (Page Number, Offset) to Physical Address.
 */
AddressTranslationResult translate_page_offset(const PageTable *pt, const MemoryConfig *config, int page_num, int offset) {
    AddressTranslationResult result;
    memset(&result, 0, sizeof(result));

    result.page_number = page_num;
    result.offset = offset;
    result.logical_address = (page_num * config->page_size_kb) + offset;

    /* Validation checks */
    if (page_num < 0 || page_num >= pt->num_pages) {
        result.is_valid_translation = false;
        snprintf(result.error_message, sizeof(result.error_message),
                 "Invalid Page Number (%d). Process P%d page table limit is 0 to %d.",
                 page_num, pt->process_id, pt->num_pages - 1);
        return result;
    }

    if (offset < 0 || offset >= config->page_size_kb) {
        result.is_valid_translation = false;
        snprintf(result.error_message, sizeof(result.error_message),
                 "Offset Boundary Error (%d). Offset must be >= 0 and < Page Size (%d KB).",
                 offset, config->page_size_kb);
        return result;
    }

    bool is_valid = false;
    int frame_num = lookup_page(pt, page_num, &is_valid);

    if (!is_valid || frame_num == -1) {
        result.is_valid_translation = false;
        snprintf(result.error_message, sizeof(result.error_message),
                 "PAGE FAULT! Page %d is not mapped to physical RAM (Valid Bit = 0).", page_num);
        return result;
    }

    result.frame_number = frame_num;
    result.physical_address = (frame_num * config->page_size_kb) + offset;
    result.is_valid_translation = true;
    return result;
}

/**
 * Translates raw Logical Address to Physical Address.
 */
AddressTranslationResult translate_logical_address(const PageTable *pt, const MemoryConfig *config, int logical_addr) {
    if (logical_addr < 0) {
        AddressTranslationResult res;
        memset(&res, 0, sizeof(res));
        res.is_valid_translation = false;
        snprintf(res.error_message, sizeof(res.error_message), "Logical address cannot be negative.");
        return res;
    }

    int page_num = logical_addr / config->page_size_kb;
    int offset = logical_addr % config->page_size_kb;

    return translate_page_offset(pt, config, page_num, offset);
}

/**
 * Interactive Paging Simulation Runner.
 */
void run_paging_demo(MemoryConfig *config, PageTable *pt) {
    print_header("MODULE 2: PAGING SIMULATION & ADDRESS TRANSLATION");

    display_page_table(pt);

    printf("Enter Page Number (0 - %d): ", pt->num_pages - 1);
    int page_num = read_int_input();

    printf("Enter Offset (0 - %d KB): ", config->page_size_kb - 1);
    int offset = read_int_input();

    printf("\nPerforming MMU Address Translation...\n");
    print_separator();

    AddressTranslationResult res = translate_page_offset(pt, config, page_num, offset);

    printf(" Logical Address Details:\n");
    printf("   Page Number      : %d\n", page_num);
    printf("   Offset           : %d KB\n", offset);
    printf("   Computed Logical : %d KB\n", res.logical_address);
    print_separator();

    if (res.is_valid_translation) {
        print_success("ADDRESS TRANSLATION SUCCESSFUL!");
        printf("   Frame Number     : %d\n", res.frame_number);
        printf("   Calculation      : (Frame %d * Page Size %d KB) + Offset %d\n",
               res.frame_number, config->page_size_kb, res.offset);
        printf("   PHYSICAL ADDRESS : %d KB\n", res.physical_address);
    } else {
        print_error(res.error_message);
    }
    print_separator();
}
