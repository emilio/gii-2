%{

#include <stdio.h>

// Assume an empty file has 1 line
static int line_count = 1;
static int word_count = 0;
static int char_count = 0;

%}

%%

[\n] { line_count++; char_count++; }
[^ \n\t]+ { word_count++; char_count += yyleng; }
. { char_count++; }

%%

int main(int argc, char** argv) {
	FILE* in = stdin;

	if ( argc > 1 )
		in = fopen(argv[1], "rb");

	if ( ! in ) {
		fprintf(stderr, "Bad input");
		exit(1);
	}

	yyin = in;

	yylex();

	printf("%d chars forming %d words in %d lines\n", char_count, word_count, line_count);

	fclose(in);

	return 0;
}
