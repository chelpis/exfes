CC = gcc

CFLAGS = -std=c99
CFLAGS += -Wall -Wextra -Wpedantic
CFLAGS += -O3
CFLAGS += -Ilib/

LIB_OBJS = \
exfes.o \

OBJS = $(foreach o,$(LIB_OBJS),objs/lib/$(o))

SCRIPTS = test5.sh test6.sh

TESTS = test1 test2 test3 test4 test5 test6

SAMPLE = sample1

all: $(SAMPLE) $(TESTS) $(SCRIPTS)

$(SAMPLE): %:objs/samples/%.o $(OBJS) | DIRS
	@echo 'Linking $@...'
	$(CC) -o bin/$@ $< $(OBJS)

$(TESTS): %:objs/tests/%.o $(OBJS) | DIRS
	@echo 'Linking $@...'
	$(CC) -fopenmp -o bin/$@ $< $(OBJS)

objs/tests/%.o: tests/%.c $(OBJS) | DIRS
	@echo 'Building $@...'
	$(CC) $(CFLAGS) -fopenmp -c $< -o $@

objs/samples/%.o: samples/%.c $(OBJS) | DIRS
	@echo 'Building $@...'
	$(CC) $(CFLAGS) -c $< -o $@

objs/lib/%.o: lib/%.c | DIRS
	@echo 'Building $@...'
	$(CC) $(CFLAGS) -c $< -o $@

$(SCRIPTS):
	@cp -f tests/$@ bin/$@

DIRS:
	@mkdir -p objs
	@mkdir -p objs/lib
	@mkdir -p objs/tests
	@mkdir -p objs/samples
	@mkdir -p bin

.PHONY: clean
clean:
	rm -rf objs bin
