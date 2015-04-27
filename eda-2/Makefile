filter_out = $(foreach v,$(2),$(if $(findstring $(strip $(1)),$(v)),,$(v)))

C_SOURCES := $(patsubst ./%, %, $(shell find -type f -name \*.c))
C_HEADERS := $(patsubst ./%, %, $(shell find -type f -name \*.h))
C_OBJECTS := $(patsubst src/c/%.c, bin/c/%.o, $(C_SOURCES))
C_EXEC_SOURCES := $(call filter_out, /lib/, $(C_SOURCES))
C_EXECS := $(patsubst src/c/%.c, target/c/%, $(C_EXEC_SOURCES))

FLAGS := -Wall
CFLAGS := --std=c99
PERCENT := %
PWD := $(shell pwd)
CC := clang

all: $(C_EXECS)
	@echo > /dev/null
debug:
	@echo "C_EXEC_SOURCES: $(C_EXEC_SOURCES)"
	@echo "C_SOURCES: $(C_SOURCES)"
	@echo "C_OBJECTS: $(C_OBJECTS)"

.SECONDEXPANSION:
$(C_OBJECTS): $$(patsubst bin/c/%.o, src/c/%.c, $$@) $$(filter $$(patsubst bin/c/%.o, src/c/%.h, $$@), $$(C_HEADERS))
	@echo "$@: $^"
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(CFLAGS) -c $< -o $@

.SECONDEXPANSION:
$(C_EXECS): $$(patsubst target/c/%, bin/c/%.o, $$@) $$(filter $$(join $$(dir $$(patsubst target/c/%, bin/c/%.o, $$@)), $(join lib/, $(PERCENT))), $$(C_OBJECTS))
	@echo "$@: $^"
	mkdir -p $(dir $@)
	$(CC) $(FLAGS) $(CFLAGS) -o $@ $^

clean:
	cd $(PWD)/bin && find -type f -name \*.o -delete
	cd $(PWD)/target && find -type f -delete
