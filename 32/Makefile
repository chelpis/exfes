CXX = gcc

CXXFLAGS = -std=c11
CXXFLAGS += -Wall
CXXFLAGS += -Wextra
CXXFLAGS += -g
CXXFLAGS += -O3

SRC_FILES = $(wildcard *.c *.cpp)
DEP_FILES = $(wildcard *.c *.cpp *.h Makefile)

test: $(DEP_FILES)
	$(CXX) $(CXXFLAGS) $(SRC_FILES) -o test

.PHONY: clean
clean:
	rm -f test
