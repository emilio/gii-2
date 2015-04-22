CC := clang
CFLAGS := -Wall -Ilib
TARGETS := target/app
C_FILES := $(wildcard src/**/*.c) $(wildcard src/*.c)
OBJECTS := $(patsubst src/%, bin/%, $(C_FILES:.c=.o))

LIB_FILES := $(wildcard lib/**/*.c) $(wildcard lib/*.c)
LIB_OBJECTS := $(patsubst lib/%, bin/lib/%, $(LIB_FILES:.c=.o))


all:  $(TARGETS)
	@echo > /dev/null

target/%: bin/%.o $(OBJECTS) $(LIB_OBJECTS)
	@mkdir -p $(dir $@)
	@echo $(LIB_FILES)
	$(CC) $(CFLAGS) -o $@ $^

src/%.c: src/%.h
	@echo > /dev/null

bin/lib/%.o: lib/%.c lib/%.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: src/%.c
	@mkdir -p bin
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -r bin
	rm -r target

.PHONY: all clean
