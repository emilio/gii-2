CC := gcc
CFLAGS := -Wall -m32 -L. -lm -lpist -D__DEBUG

all: pist2
	@echo > /dev/null

pist2: pist2.c pist2.h
	$(CC) $(CFLAGS) $< libpist.a -o $@ -lm

clean:
	rm pist2

