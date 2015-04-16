CC := gcc
CFLAGS := -Wall

all: target/lexer
	@echo > /dev/null

target/lexer: target/lex.yy.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -lfl $< -o $@

target/tokens.h: src/tokens.h
	@mkdir -p $(dir $@)
	cp $< $@

target/lex.yy.c: src/lexer.l target/tokens.h
	@mkdir -p $(dir $@)
	flex -o $@ $<
