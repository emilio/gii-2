ifeq ($(TARGET_ENV),oracle)
PREPRO := proc MODE=ANSI iname=
CC := gcc
else
CC := clang
PREPRO := ecpg
CLINKFLAGS := -lecpg
CFLAGS := -I/usr/include/postgresql -DPOSTGRES
endif
CFLAGS := $(CFLAGS) -Wall -Ilib
TARGETS := target/app
SC_FILES := $(wildcard src/**/*.sc) $(wildcard src/*.sc)
C_PREPROCESSED_FILES := $(SC_FILES:.sc=.c)
C_FILES := $(C_PREPROCESSED_FILES) $(wildcard src/**/*.c) $(wildcard src/*.c)
OBJECTS := $(patsubst src/%, bin/%, $(C_FILES:.c=.o))

LIB_FILES := $(wildcard lib/**/*.c) $(wildcard lib/*.c)
LIB_OBJECTS := $(patsubst lib/%, bin/lib/%, $(LIB_FILES:.c=.o))


all: $(TARGETS)
	@echo > /dev/null

database:
	$(MAKE) -C etc/sql

target/%: bin/%.o $(OBJECTS) $(LIB_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(CLINKFLAGS)

src/%.c: src/%.sc
	$(PREPRO) -c $< -o $@

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
