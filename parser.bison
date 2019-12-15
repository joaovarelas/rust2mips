 // Use "%code requires" to make declarations go
 // into both parser.c and parser.h
%code requires {
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "ast.h"
    #include "table.h"
    
    extern int yylex();
    extern int yyline;
    extern char* yytext;
    extern FILE* yyin;
    extern void yyerror(const char* msg);
    
    AST* root;
}
			
			
//  Tokens
%token			fn fmain
%token			let mut IF ELSE WHILE println readln
			
%token			number symbol string
%token			plus minus mult divi mod
%token			not and or eqt grt geq lrt leq neq

%token			equal commentline quote

%token			'(' ')' '{' '}' ';'
			
// Operator associativity & precedence
%left			lrt grt eqt neq leq geq
%left			and or not
%left			plus minus
%left			mult divi mod

			
// Types/values in association to grammar symbols.
%union {
    AST* ast;
    int number;
    Symbol* symbol;
    char* string;
}
			
			
%type	<ast>		main expr statements statement
			assignment if_statement while_statement
			io_func commentline comments fprintln freadln
			
%type	<number>	number
%type	<symbol>	symbol
%type	<string>	string
					
			
			
 // Root-level grammar symbol
%start program;
			
			
%%

program: 	main { } ;
		
main: 		fn fmain '(' ')' '{' statements '}' { root = $6; } 
		
statements:   	statement statements { $$ = mk_ast(CMD, $1, $2); }
	|	{ $$ = NULL; }
  
		
statement:   	assignment | if_statement | while_statement | io_func | comments ;
		
		
assignment:
		let symbol equal expr ';' { $$ = mk_assign($2, $4); }
	| 	let mut symbol equal expr ';' { $$ = mk_assign($3, $5); }
	| 	symbol equal expr ';' { $$ = mk_assign($1, $3); }
         
		
		
if_statement:
		IF expr '{' statements '}' { $$ = mk_flow(IFS, $2, $4, NULL); }
	| 	IF expr '{' statements '}' ELSE '{' statements '}' { $$ = mk_flow(IFS, $2, $4, $8); }
	| 	IF expr '{' statements '}' ELSE if_statement { $$ = mk_flow(IFS, $2, $4, $7); }
         	
		

while_statement:
		WHILE expr '{' statements '}' { $$ = mk_flow(WHS, $2, $4, NULL); } 
		
		
io_func:  	fprintln | freadln ;
	       	

		
fprintln:
		println '(' string ')' ';' { $$ = mk_func(PTL, NULL, 0, $3); }
	| 	println '(' symbol ')' ';' { $$ = mk_func(PTL, $3, 0, NULL); }
	| 	println '(' number ')' ';' { $$ = mk_func(PTL, NULL, $3, NULL); }

		
freadln:  	readln '(' symbol ')' ';' { $$ = mk_func(RDL, $3, 0, NULL); }
	|	let symbol equal readln '(' ')' ';' { $$ = mk_func(RDL, $2, 0, NULL); }


expr:
		expr and expr { $$ = mk_ast(AND, $1, $3); }
	| 	expr or  expr { $$ = mk_ast(OR, $1, $3); }
	| 	expr eqt expr { $$ = mk_ast(EQT, $1, $3); }
	| 	expr neq expr { $$ = mk_ast(NEQ, $1, $3); }
	| 	expr grt expr { $$ = mk_ast(GRT, $1, $3); }
	| 	expr geq expr { $$ = mk_ast(GEQ, $1, $3); }
	| 	expr lrt expr { $$ = mk_ast(LRT, $1, $3); }
	| 	expr leq expr { $$ = mk_ast(LEQ, $1, $3); }
		
	|	expr plus expr { $$ = mk_ast(ADD, $1, $3); }
	| 	expr mult expr { $$ = mk_ast(MULT, $1, $3); }
	| 	expr minus expr { $$ = mk_ast(SUB, $1, $3); }
	| 	expr divi expr { $$ = mk_ast(DIV, $1, $3); }
	| 	expr mod expr { $$ = mk_ast(MOD, $1, $3); }

	| 	number { $$ = mk_num($1); }
	| 	minus number { $$ = mk_num($2 * -1); }
	| 	symbol { $$ = mk_sym($1); }
         	
comments:   	commentline

%%

void yyerror(const char* err) {
    printf("Parser line %d: %s - '%s'\n", yyline, err, yytext);
}
