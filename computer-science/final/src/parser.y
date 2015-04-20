%{

#define YYERROR_VERBOSE
#define YYSTYPE struct node *

struct node;

struct node* node_new(const char* const, int, ...);
struct node* node_append(struct node*, int, ...);
struct node* node_nil();
struct node* atom_new(const char* const);

extern char *yytext;
extern int yylex();
int yyerror(const char*);

%}
%debug

%token INTEGER_LITERAL
%token LET
%token IDENT
%token IF
%token ELSE
%token EQEQ
%token NIL


%left EQEQ
%left '+' '-'
%left '*' '/'

%precedence '(' '{' '}' ')'

%%
program
: block
| maybe_statements
;

block
: '{' maybe_statements '}'   { $$ = node_new("Block", 1, $2);  }
;

maybe_statements
: statements
| statements expression     { $$ = node_append($1, 1, $2); }
| expression
| %empty					{ $$ = node_nil(); }
;

statements
: statement
| statements statement
;

statement
: expression ';'
| assignment
;

assignment
: LET ident '=' expression ';' { $$ = node_new("Assignment", 2, $2, $4); }
;

expression
: literal
| ident
| expression_operator
| '(' expression ')'           { $$ = $2; }
| block
;

expression_operator
: expression '-' expression    { $$ = node_new("BinaryOp", 3, atom_new("BinarySub"), $1, $3); }
| expression '+' expression    { $$ = node_new("BinaryOp", 3, atom_new("BinaryAdd"), $1, $3); }
| expression '*' expression    { $$ = node_new("BinaryOp", 3, atom_new("BinaryMul"), $1, $3); }
| expression '/' expression    { $$ = node_new("BinaryOp", 3, atom_new("BinaryDiv"), $1, $3); }
| expression '<' expression    { $$ = node_new("BinaryOp", 3, atom_new("BinaryLt"), $1, $3); }
| expression '>' expression    { $$ = node_new("BinaryOp", 3, atom_new("BinaryGt"), $1, $3); }
| expression EQEQ expression   { $$ = node_new("BinaryOp", 3, atom_new("BinaryEq"), $1, $3); }
;

ident
: IDENT						{ $$ = node_new("ident", 1, atom_new(yytext)); }
;

literal
: INTEGER_LITERAL           { $$ = node_new("Integer", 1, atom_new(yytext)); }
| NIL                       { $$ = node_nil(); }
;
