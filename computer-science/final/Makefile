CC := clang
CFLAGS := -Wall
LINKFLAGS := -lfl

.PHONE: all clean
all: target/parser
	@echo > /dev/null

clean:
	rm target/*

# Custom files
target/%: src/%
	@mkdir -p $(dir $@)
	cp $< $@

# Lexer
target/lex.yy.c: src/lexer.l target/tokens.h target/y.tab.h
	@mkdir -p $(dir $@)
	flex -o $@ $<

target/lexer.o: target/lex.yy.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(LINKFLAGS)

# Parser
target/y.tab.h: target/y.tab.c
	@mkdir -p $(dir $@)

target/y.tab.c: src/parser.y
	@mkdir -p $(dir $@)
	bison -d $< -o $@

target/parser.o: target/y.tab.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LINKFLAGS)

# Main parse step
target/main.o: target/main.c target/parser.o
	$(CC) $(CFLAGS) -c $< -o $@ target/parser.o $(LINKFLAGS)

target/parser: target/main.o target/parser.o target/lexer.o
	$(CC) $(CFLAGS) -o $@ $^ $(LINKFLAGS)
