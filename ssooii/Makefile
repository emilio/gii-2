CC := gcc
INCLUDES := -Ilib -I/usr/include/GL
LIBS := -lpthread -lglut -lGL -lGLU -lGLEW
CFLAGS := -Wall $(INCLUDES)
EXECUTABLE_C_FILES := $(wildcard *.c)
LIB_C_FILES := $(wildcard lib/**/*.c)

EXECUTABLES := $(EXECUTABLE_C_FILES:%.c=target/%)
LIB_OBJECT_FILES := $(LIB_C_FILES:lib/%.c=bin/%.o)

all: $(EXECUTABLES)
	@echo > /dev/null

debug:
	@echo $(EXECUTABLES)
	@echo $(LIB_C_FILES)
	@echo $(LIB_OBJECT_FILES)

clean:
	rm -r bin/ target/

bin/%.o: %.c
	@mkdir -p bin
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

bin/%.o: lib/%.c
	@mkdir -p bin
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

target/%: bin/%.o $(LIB_OBJECT_FILES)
	@mkdir -p target
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
