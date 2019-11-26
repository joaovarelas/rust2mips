// Code definitions
#ifndef __code_h__
#define __code_h__

extern void yyerror(const char* msg);


typedef
enum {
      ATRIB, PLUS, MINUS, DIV, MULT, LABEL, GOTO, IFE, IFG, IFGE, LFL, IFLE, IFDIF
} OpKind;


typedef
enum {
      STR, INT, EMPTY
} AtomKind;


typedef
struct {
    AtomKind kind;
    union {
	int valor;
	char* name;
    } u;
} Atom;


typedef
struct {
    OpKind op;
    Atom el1, el2, el3, el4;
} Instr;
