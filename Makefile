CC = gcc
CFLAGS = -Wall -std=c99 -g
VFLAGS = --leak-check=full --show-leak-kinds=all

all : bin bin/dict.o bin/ignored.o bin/files.o bin/test.o bin/main.o test bin/translator

.PHONY : bin
bin:
	mkdir -p bin

bin/files.o:src/files.c
	$(CC) $(CFLAGS) -c $< -o $@

bin/test.o:src/test.c
	$(CC) $(CFLAGS) -c $< -o $@

bin/dict.o:src/dict.c
	$(CC) $(CFLAGS) -c $< -o $@

bin/ignored.o:src/ignored.c
	$(CC) $(CFLAGS) -c $< -o $@

bin/main.o:src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

bin/translator:bin/main.o bin/dict.o bin/files.o bin/ignored.o
	$(CC) $(CFLAGS) $^ -o $@

test:bin/test.o bin/dict.o bin/ignored.o bin/files.o
	$(CC) $(CFLAGS) $^ -o $@

.PHONY : clean
clean :
	rm -f bin/*.o src/*~ src/*# src/.# ./*~ .#* *# test bin/translator

.PHONY : valgrind
valgrind :
	valgrind $(VFLAGS) bin/translator
