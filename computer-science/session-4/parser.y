%{
	#include <stdio.h>
%}
%token IDENT
%token LIT
%%
statement: expression ';' {printf ("=%d\n",$1);}
	;
expression: expression '+' expression {$$ = $1 + $3;}
        | expression '-' expression {$$ = $1 - $3; }
        | expression '*' expression { $$ = $1 - $3; }
        | LIT {$$ = $1; }
         ;

%%
int main() {
    yyparse();
    yylex();
    return 0;
}
void yyerror(char *s) {fprintf(stderr, "%s\n",s);}


