CC := gcc
CFLAGS := -Wall -m32 -D__DEBUG -O3
LINK_FLAGS := -L. -lpist -lm
all: pist2
	@echo > /dev/null

pist2: pist2.c pist2.h
	$(CC) $(CFLAGS) $< -o $@ $(LINK_FLAGS)

clean:
	rm pist2

