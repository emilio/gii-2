%{

#define YYERROR_VERBOSE
#define YYSTYPE struct node *
struct node;

%}
%debug

%token INTEGER_LITERAL
%token LET
%token IDENT
%token IF
%token ELSE


%left '+' '-'
%left '*' '/'

%%

block
: '{' maybe_statements '}'   { $$ = node_new("Block", 1, $2);  }
;

maybe_statements
: statements
| statenents expression     { $$ = node_append($1, 1, $2); }
| expression
| %empty					{ $$ = node_nil(); }
;

statements
: statement
| statements statement
;

statement
: expression ';'
| block
;

expression
: literal
| ident
| expression_operator
;

expression_operator
: expresion '-' expression    { $$ = node_new("BinaryOp", 3, atom_new("BinarySub"), $1, $3); }
| expresion '+' expression    { $$ = node_new("BinaryOp", 3, atom_new("BinaryAdd"), $1, $3); }
| expresion '*' expression    { $$ = node_new("BinaryOp", 3, atom_new("BinaryMul"), $1, $3); }
| expresion '/' expression    { $$ = node_new("BinaryOp", 3, atom_new("BinaryDiv"), $1, $3); }
| expresion '<' expression    { $$ = node_new("BinaryOp", 3, atom_new("BinaryLt"), $1, $3); }
| expresion '>' expression    { $$ = node_new("BinaryOp", 3, atom_new("BinaryGt"), $1, $3); }
| expresion EQEQ expression    { $$ = node_new("BinaryOp", 3, atom_new("BinaryEq"), $1, $3); }
;

ident
: IDENT						{ $$ = node_new("ident", 1, atom_new(yytext)); }
;

literal
: INTEGER_LITERAL           { $$ = node_new("Integer", 1, atom_new(yytext)); }
;
