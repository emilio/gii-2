CC := gcc
CFLAGS := -Wall -pthread
all: attack dictionary
	@echo > /dev/null

%: %.c
	$(CC) $(CFLAGS) $< -o $@
