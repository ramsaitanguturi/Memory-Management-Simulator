# Compiler & Flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Source & Object Files
SRCS = main.c \
       memory.c \
       paging.c \
       segmentation.c \
       fifo.c \
       lru.c \
       optimal.c \
       page_table.c \
       comparison.c \
       utils.c \
       gantt_memory.c

OBJS = $(SRCS:.c=.o)

# Executable Output Name
TARGET = memory_simulator

# OS Detection for Executable Extension
ifeq ($(OS),Windows_NT)
    EXEC = $(TARGET).exe
    RM = del /Q
else
    EXEC = $(TARGET)
    RM = rm -f
endif

# Default Target
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) $(EXEC) memory_report.txt

run: $(EXEC)
	./$(EXEC)

.PHONY: all clean run
