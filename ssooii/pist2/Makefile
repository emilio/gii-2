CC := gcc
CFLAGS := -Wall -m32 -D__DEBUG -O3
LINK_FLAGS := -L. -lpist -lm
DEPS := pist2.c pist2.h


all: pist2 conformant
	@echo > /dev/null

conformant: pist2_conformant
	@echo > /dev/null

pist2_conformant: $(DEPS)
	$(CC) $(CFLAGS) -DSTATEMENT_CONFORMANT $< -o $@ $(LINK_FLAGS)

pist2: $(DEPS)
	$(CC) $(CFLAGS) $< -o $@ $(LINK_FLAGS)

clean:
	rm pist2

.PHONY: all clean conformant
