CC := clang
CFLAGS := -Wall -m32 -D__DEBUG -O3 -pedantic
LINK_FLAGS := -L. -lpist -lm
DEPS := pist2.c pist2.h


all: pist2 bypass
	@echo > /dev/null

bypass: pist2_bypassing
	@echo > /dev/null

pist2: $(DEPS)
	$(CC) $(CFLAGS) $< -o $@ $(LINK_FLAGS)

pist2_bypassing: $(DEPS)
	$(CC) $(CFLAGS) $< -DDEBUG -DBYPASS_STATEMENT -o $@ $(LINK_FLAGS)

clean:
	rm pist2_bypassing
	rm pist2

.PHONY: all clean bypass
