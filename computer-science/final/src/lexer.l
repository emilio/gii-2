%{

#include <stdio.h>
#include <ctype.h>
#include "y.tab.h"

%}

%option stack
%option yylineno
%option noyywrap

%s line_comment
%s block_comment

ident [a-zA-Z_][0-9a-zA-Z_]*

%%

\/\/                 { BEGIN(line_comment); }
<line_comment>\n     { BEGIN(INITIAL); }
<line_comment>[^\n]* {}

\/\*                 { BEGIN(block_comment); }
<block_comment>\*\/  { BEGIN(INITIAL); }
<block_comment>.|\n  {}

0o[0-9]+             { return INTEGER_LITERAL; }
0x[0-9A-Za-z]+       { return INTEGER_LITERAL; }
0b[01]+              { return INTEGER_LITERAL; }
[0-9]+               { return INTEGER_LITERAL; }

\(    { return '('; }
\)    { return ')'; }
\{    { return '{'; }
\}    { return '}'; }
\[    { return '['; }
\]    { return ']'; }

\+    { return '+'; }
\-    { return '-'; }
\*    { return '*'; }
\/    { return '/'; }

==    { return EQEQ; }

{ident}  { return IDENT; }

\s    {}

<<EOF>>  { return 0; }

%%
