SOURCES := $(wildcard *.l)
TARGETS := $(SOURCES:.l=)

all: $(TARGETS)
	@echo > /dev/null

%: %.yy.c
	$(CC) -Wall $< -o $@ -ll

%.yy.c: %.l
	lex -o $@ $<
