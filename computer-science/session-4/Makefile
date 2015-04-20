
all: parser
	@echo > /dev/null

parser: parser.y
	bison -d $<
	$(CC) -Wall $@.tab.c -o $@ -lfl -ll
