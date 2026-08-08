/**
 * @file utils.h
 * @brief Utility Functions, Terminal Formatting, File I/O, and Report Generation Header
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include "memory.h"
#include "page_table.h"
#include "segmentation.h"
#include "fifo.h"

/* Terminal ANSI Color Codes */
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"

/* Formatting Helpers */
void print_header(const char *title);
void print_separator(void);
void print_success(const char *msg);
void print_error(const char *msg);
void print_warning(const char *msg);

/* Input Readers */
int read_int_input(void);
void read_string_input(char *buffer, int max_len);
int parse_reference_string(const char *str_input, int *output_array, int max_size);

/* File Input & Report Export */
bool load_config_file(const char *filename, MemoryConfig *config, PageTable *pt, SegmentTable *st, int *ref_string, int *ref_len);
bool export_simulation_report(const char *filename, const MemoryConfig *config, const PageTable *pt, const SegmentTable *st, int frames_count, const int *ref_string, int ref_len);

#endif /* UTILS_H */
