%{
// HEADERS
#include <stdlib.h>
#include "parser.h"

// variables maintained by the lexical analyser
int yyline = 1;
%}

%option noyywrap

%%

[ \t]+ {  }
\/\/.* {  }
\n { yyline++; }

"fn" { return fn; }
"main" { return fmain; }

"let" { return let; }
"mut" { return mut; }
"true" { yylval.number = 1; return number; }
"false" { yylval.number = 0; return number; }
"if" { return IF; }
"else" { return ELSE; }
"while" { return WHILE; }

"println!" { return println; }
"read_line" { return readln; }

"!" { return not; }
"&&" { return and; }
"||" { return or; }
"==" { return eqt; }
"!=" { return neq; }
"<" { return lrt; }
">" { return grt; }
"<=" { return leq; }
">=" { return geq; }
"=" { return equal; }
"{" { return '{'; }
"}" { return '}'; }
"(" { return '('; }
")" { return ')'; }
";" { return ';'; }

"+" { return plus; }
"-" { return minus; }
"*" { return mult; }
"/" { return divi; }
"%" { return mod; }


[0-9]+ { yylval.number = atoi(yytext); return number; }
[a-zA-Z][a-zA-Z0-9]* {  yylval.symbol = search_table(yytext); return symbol; }
\".*\" { yylval.string = strdup(yytext); return string; }


.  { yyerror("Lexer: unexpected character"); }
%%
