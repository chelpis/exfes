CC = gcc

CFLAGS = -std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -O3

SRC_FILES = $(wildcard *.c *.cpp)
DEP_FILES = $(wildcard *.c *.cpp *.h Makefile)

test: $(DEP_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES) -o test

.PHONY: clean
clean:
	rm -f test
