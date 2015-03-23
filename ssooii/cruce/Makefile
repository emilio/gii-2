CC := gcc
CFLAGS := -Wall -m32 -D__DEBUG -O3
LINK_FLAGS := -L. -lcruce -lm
all: cruce
	@echo > /dev/null

cruce: cruce.c cruce.h
	$(CC) $(CFLAGS) $< -o $@ $(LINK_FLAGS)

clean:
	rm pist2

