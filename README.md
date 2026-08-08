# Memory Management Simulator

A complete, 3rd-year Computer Science Operating Systems course project written in standard C (**C99 compliant**). This application simulates the core **Memory Management Subsystem** of a modern Operating System, modeling **Paging**, **Segmentation**, **MMU Address Translation**, and **Page Replacement Algorithms (FIFO, LRU, Optimal)** with step-by-step trace analysis and performance comparisons.

---

## Table of Contents
- [Project Overview](#project-overview)
- [Operating System Concepts Implemented](#operating-system-concepts-implemented)
  - [1. Physical Memory Configuration & Frames](#1-physical-memory-configuration--frames)
  - [2. Paging & MMU Address Translation](#2-paging--mmu-address-translation)
  - [3. Segmentation & Boundary Checks (Segmentation Faults)](#3-segmentation--boundary-checks-segmentation-faults)
  - [4. Page Replacement Algorithms](#4-page-replacement-algorithms)
  - [5. Performance Comparison Matrix](#5-performance-comparison-matrix)
- [Project Architecture & File Structure](#project-architecture--file-structure)
- [Compilation & Execution Guide](#compilation--execution-guide)
  - [Linux / Ubuntu](#linux--ubuntu)
  - [Windows (MinGW / GCC)](#windows-mingw--gcc)
  - [Direct GCC Compilation](#direct-gcc-compilation)
- [Sample Execution Traces](#sample-execution-traces)
- [Additional Features](#additional-features)
- [Edge Cases Handled](#edge-cases-handled)
- [Academic Context & Future Enhancements](#academic-context--future-enhancements)

---

## Project Overview

In an operating system, the **Memory Management Unit (MMU)** and kernel subsystem coordinate physical RAM allocations, process virtual address spaces, and secondary storage paging (swap space). 

This simulator abstracts physical memory into discrete frames and models:
1. How virtual addresses mapped via **Page Tables** or **Segment Tables** translate into physical RAM addresses.
2. How page faults occur when virtual pages are not present in physical RAM.
3. How page replacement policy choices (**FIFO**, **LRU**, **Optimal**) impact overall system hit ratios and page fault frequencies.

---

## Operating System Concepts Implemented

### 1. Physical Memory Configuration & Frames
Physical memory (RAM) is divided into fixed-size contiguous blocks called **Frames**. Logical memory requested by processes is partitioned into equal-sized blocks called **Pages**.
- **Total Physical Memory**: Configurable (e.g. 1024 KB).
- **Page / Frame Size**: Configurable (e.g. 64 KB).
- **Total Frames ($N$)**: 
  $$\text{Total Frames} = \frac{\text{Total Memory (KB)}}{\text{Page Size (KB)}}$$
  For 1024 KB RAM and 64 KB Page Size, $\text{Total Frames} = 16$ (numbered $0, 1, 2, \dots, 15$).

---

### 2. Paging & MMU Address Translation
Paging eliminates the requirement for contiguous allocation of physical memory.
- **Page Table**: Maintained per process. Maps Virtual Page Numbers (VPN) to Physical Frame Numbers (PFN) with a Valid/Present bit.
- **Logical Address Division**:
  $$\text{Logical Address} = (\text{Page Number} \times \text{Page Size}) + \text{Offset}$$
- **Physical Address Formula**:
  $$\text{Physical Address} = (\text{Frame Number} \times \text{Page Size}) + \text{Offset}$$

#### Translation Example:
- **Given**: Page Number = `3`, Offset = `20 KB`, Page Size = `64 KB`.
- **Page Table Lookup**: Page `3` $\rightarrow$ Frame `10` (Valid Bit = `1`).
- **Physical Address**: $(10 \times 64) + 20 = 640 + 20 = 660 \text{ KB}$.

---

### 3. Segmentation & Boundary Checks (Segmentation Faults)
Segmentation reflects the programmer's view of memory (organizing program modules into code, data, stack, and symbol segments).
- **Segment Table**: Contains **Base Address** (physical starting point) and **Limit** (segment size).
- **Address Translation Formula**:
  $$\text{Physical Address} = \text{Base Address} + \text{Offset} \quad \text{iff } \text{Offset} < \text{Limit}$$
- **Segmentation Fault Trap**: If $\text{Offset} \ge \text{Limit}$, the hardware MMU flags an invalid memory access boundary violation, generating a **Segmentation Fault** trap to the OS kernel.

---

### 4. Page Replacement Algorithms
When a page fault occurs and all physical memory frames are occupied, the OS must evict a victim page from RAM to swap space.

#### A. First-In, First-Out (FIFO)
- **Concept**: Evicts the page that arrived first in physical memory (oldest loaded page).
- **Data Structure**: Queue / Circular Pointer.
- **Behavior**: Easy to implement, but subject to **Belady's Anomaly** (where increasing physical frames can sometimes increase page faults).

#### B. Least Recently Used (LRU)
- **Concept**: Evicts the page that has not been referenced for the longest period in past time.
- **Data Structure**: Counter/Logical Clock timestamps or access stacks.
- **Behavior**: Approximates optimal replacement based on the **Locality of Reference** principle.

#### C. Optimal (Belady's Min Algorithm)
- **Concept**: Evicts the page that will not be used for the longest period of time in the **future**.
- **Behavior**: Guarantees the absolute lowest possible page fault rate for any frame allocation.
- **Note**: Serves as a theoretical upper-bound benchmark because real operating systems cannot look into future execution paths.

---

### 5. Performance Comparison Matrix
The simulator evaluates FIFO, LRU, and Optimal algorithms on identical reference strings and reports:
$$\text{Hit Ratio (\%)} = \left( \frac{\text{Total Hits}}{\text{Total References}} \right) \times 100$$
$$\text{Fault Ratio (\%)} = \left( \frac{\text{Total Page Faults}}{\text{Total References}} \right) \times 100$$

---

## Project Architecture & File Structure

```
Memory-Management-Simulator/
├── main.c              # Interactive CLI menu loop and subsystem coordinator
├── memory.h / .c       # Physical RAM configuration and frame allocation state
├── page_table.h / .c   # Page Table entries, mapping, unmapping, and lookup
├── paging.h / .c       # Paging simulation & MMU address translation engine
├── segmentation.h / .c # Segment Table descriptors & Segmentation Fault trap handler
├── fifo.h / .c         # First-In, First-Out Page Replacement algorithm
├── lru.h / .c          # Least Recently Used Page Replacement algorithm
├── optimal.h / .c      # Optimal (Belady's Min) Page Replacement algorithm
├── comparison.h / .c   # Algorithm comparison engine & winner evaluation
├── gantt_memory.h / .c # ASCII Physical Memory frame visualizer & box grid layout
├── utils.h / .c        # ANSI colors, input parsing, configuration file loader, report exporter
├── Makefile            # Cross-platform build script (GCC / Make / MinGW)
├── sample_input.txt    # Input configuration text file
└── README.md           # Documentation
```

---

## Compilation & Execution Guide

### Linux / Ubuntu
```bash
# Build binary
make

# Run simulator
./memory_simulator

# Clean build artifacts
make clean
```

### Windows (MinGW / GCC)
```powershell
# Build binary using MinGW Make
mingw32-make

# Run simulator
.\memory_simulator.exe

# Clean build artifacts
mingw32-make clean
```

### Direct GCC Compilation
```bash
gcc -Wall -Wextra -std=c99 main.c memory.c paging.c segmentation.c fifo.c lru.c optimal.c page_table.c comparison.c utils.c gantt_memory.c -o memory_simulator
```

---

## Sample Execution Traces

### Paging Address Translation Output
```
================================================================================
 MODULE 2: PAGING SIMULATION & ADDRESS TRANSLATION
================================================================================
+-------+-------+---------+
| Page  | Frame | Valid   |
+-------+-------+---------+
| 0     | 5     | Yes     |
| 1     | 8     | Yes     |
| 2     | 1     | Yes     |
| 3     | 10    | Yes     |
+-------+-------+---------+

[SUCCESS] ADDRESS TRANSLATION SUCCESSFUL!
   Frame Number     : 10
   Calculation      : (Frame 10 * Page Size 64 KB) + Offset 20
   PHYSICAL ADDRESS : 660 KB
--------------------------------------------------------------------------------
```

### Segmentation Boundary Violation (Segmentation Fault) Output
```
================================================================================
 MODULE 3: SEGMENTATION SIMULATION & ADDRESS TRANSLATION
================================================================================
+---------+-----------------+--------------+--------------+
| Segment | Name            | Base Address | Limit (Size) |
+---------+-----------------+--------------+--------------+
| 0       | Code Segment    | 1000         | 500          |
| 1       | Data Segment    | 2000         | 300          |
| 2       | Stack Segment   | 3000         | 700          |
+---------+-----------------+--------------+--------------+

[ERROR] TRAP TO OPERATING SYSTEM GENERATED!
[ERROR] SEGMENTATION FAULT! Offset (600) >= Segment Limit (500). OS traps process P1!
   Base Address     : 1000
   Limit Check      : Offset (600) >= Limit (500) [VIOLATION]
--------------------------------------------------------------------------------
```

### Performance Comparison Table Output
```
================================================================================
 MODULE 6: PAGE REPLACEMENT ALGORITHMS COMPARISON
================================================================================
 Test Parameters:
   Physical Frames Allocated : 3
   Reference String Length   : 8
   Reference String          : 7 0 1 2 0 3 0 4 

+------------------+-------------+------------+---------------+-----------------+
| Algorithm        | Page Faults | Page Hits  | Hit Ratio (%) | Fault Ratio (%) |
+------------------+-------------+------------+---------------+-----------------+
| FIFO             | 6           | 2          | 25.00         | 75.00           |
| LRU              | 6           | 2          | 25.00         | 75.00           |
| Optimal          | 5           | 3          | 37.50         | 62.50           |
+------------------+-------------+------------+---------------+-----------------+

[SUCCESS] PERFORMANCE ANALYSIS CONCLUSION:
   Best Performing Algorithm : Optimal
   Lowest Page Fault Count   : 5 faults
   Highest Hit Ratio         : 37.50%
--------------------------------------------------------------------------------
```

---

## Additional Features

1. **File Input Loading (`sample_input.txt`)**: Option 9 loads system specifications, page configurations, segment limits, and reference strings directly from external text files.
2. **ASCII Memory Frame Visualization**: Option 8 renders both text-based memory range lists and horizontal ASCII box layouts representing RAM frame allocation states.
3. **Simulation Report Generation (`memory_report.txt`)**: Option 10 exports comprehensive simulation reports containing memory specs, page/segment tables, step-by-step page replacement logs, and comparison matrices.

---

## Edge Cases Handled

- **Page Size Exceeds Memory**: Validates page size and physical RAM parameters during configuration setup.
- **Non-Power-of-2 Page Sizes**: Emits standard OS warnings regarding non-power-of-two page sizes.
- **Offset Out of Bounds**: Checks if $\text{Offset} \ge \text{Page Size}$ during paging translation.
- **Segmentation Limit Violations**: Detects $\text{Offset} \ge \text{Limit}$ and raises simulated hardware Segmentation Fault traps.
- **Zero / Negative Inputs**: Input sanitization prevents division by zero or invalid array accesses.
- **Empty Reference Strings & 0 Frames**: Guarded against null dereferences and division by zero.

---

## Academic Context & Future Enhancements

This project was built for **3rd-Year Computer Science Operating Systems Coursework**. It clearly demonstrates how modern OS kernels manage memory abstraction, address translation, and page fault replacement strategies.

### Future Enhancements
- Implementation of **Second-Chance (Clock)** Page Replacement Algorithm.
- Simulation of **Multi-level Paging** (Two-Tier Page Tables for 32-bit/64-bit architectures).
- Simulation of **TLB (Translation Lookaside Buffer)** caching hits and misses.
