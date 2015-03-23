%{

#include <stdio.h>

// Assume an empty file has 1 line
static int line_count = 1;
static int word_count = 0;
static int char_count = 0;

%}

%%

[0-9]*\.[0-9+]|[0-9]+\.[0-9]* { printf("{REAL, %s}\n", yytext); }
(0x[0-9A-Fa-f]+|0[0-7]+|[0-9]+) { printf("{INTEGER_LITERAL, %s}\n", yytext); }
-|\+|\/|\*|\||&|= { printf("{OPERATOR, %s}\n", yytext); }
[ \n\t] {}
. { return -1; }
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

	return 0;
}
