CC = gcc

CFLAGS = -std=c99
CFLAGS += -Wall -Wextra -Wpedantic
CFLAGS += -O3
CFLAGS += -Ilib/

sample1: objs/exfes.o samples/sample1.c | DIRS
	$(CC) $(CFLAGS) objs/exfes.o samples/sample1.c -o bin/sample1

testCheck: objs/exfes.o tests/testCheck.c | DIRS
	$(CC) $(CFLAGS) objs/exfes.o tests/testCheck.c -o bin/testCheck

test1: objs/exfes.o tests/test1.c | DIRS
	$(CC) $(CFLAGS) objs/exfes.o tests/test1.c -o bin/test1

test2: objs/exfes.o tests/test2.c | DIRS
	$(CC) $(CFLAGS) objs/exfes.o tests/test2.c -o bin/test2

test3: objs/exfes.o tests/test3.c | DIRS
	$(CC) $(CFLAGS) objs/exfes.o tests/test3.c -o bin/test3

test4: objs/exfes.o tests/test4.c | DIRS
	$(CC) $(CFLAGS) objs/exfes.o tests/test4.c -o bin/test4

test5: objs/exfes.o tests/test5.c | DIRS
	$(CC) $(CFLAGS) objs/exfes.o tests/test5.c -fopenmp -o bin/test5

test6: objs/exfes.o tests/test6.c | DIRS
	$(CC) $(CFLAGS) objs/exfes.o tests/test6.c -fopenmp -o bin/test6

memCheck: tests/test5.sh tests/test6.sh
	cp tests/test5.sh tests/test6.sh bin/

objs/exfes.o: lib/exfes.c | DIRS
	$(CC) $(CFLAGS) -c lib/exfes.c -o objs/exfes.o

all: sample1 testCheck test1 test2 test3 test4 test5 test6 memCheck

.PHONY: DIRS
DIRS:
	mkdir -p objs
	mkdir -p bin

.PHONY: check
check:
	./bin/testCheck

.PHONY: clean
clean:
	rm -rf objs bin
