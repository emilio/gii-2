CC := gcc
CFLAGS := -Wall
TARGETS := target/app
C_FILES := $(wildcard src/*.c)
OBJECTS := $(patsubst src/%, bin/%, $(C_FILES:.c=.o))


all:  $(TARGETS)
	@echo > /dev/null

target/%: bin/%.o $(OBJECTS)
	@mkdir -p target
	$(CC) $(CFLAGS) -o $@ $^

src/%.c: src/%.h
	@echo > /dev/null

bin/%.o: src/%.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -r bin
	rm -r target

.PHONY: all clean
