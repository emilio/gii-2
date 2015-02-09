CC = gcc
CFLAGS = -Wall

all: list_test
	@echo > /dev/null

list_test: list_test.o list.o
	$(CC) $^ -o $@ $(CFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)
