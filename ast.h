// AST definitions
#ifndef __ast_h__
#define __ast_h__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // for malloc
#include <string.h>
#include <limits.h>


extern void yyerror(const char* msg);

typedef
enum _NodeType {
		ADD, SUB, MULT, DIV, MOD, // [0 - 4] ARITHMETIC
		GRT, GEQ, LRT, LEQ, EQT, NEQ, // [5 - 10] RELATIONAL
                AND, OR, NOT, // [11 - 13] LOGIC
		CMD, ASG, IFS, WHS, PTL, RDL, // [14 - 19] CMDS
		NUM, SYM // [20 - 21] TERMINAL SYMBOLS
} Type;


typedef enum { ARI, REL, LOG, STM, TRM } Types;
const static
int type_map[] = { ARI, ARI, ARI, ARI, ARI,
                   REL, REL, REL, REL, REL, REL,
                   LOG, LOG, LOG,  
                   STM, STM, STM, STM, STM, STM,
                   TRM, TRM };

    
// Tree Struct
struct _AST {
    int type;
    struct _AST* left;
    struct _AST* right;
};
typedef struct _AST AST;



// Terminal Symbols
struct _IntVal {
    int type;
    int num;
};
typedef struct _IntVal IntVal;

struct _Symbol {
    char* name;
    int val;
};
typedef struct _Symbol Symbol;

struct _SymbolRef {
    int type;
    Symbol* sym;
};
typedef struct _SymbolRef SymbolRef;


// Statements Struct
struct _AssignVal {
    int type;
    Symbol* sym;
    AST* val;
};
typedef struct _AssignVal AssignVal;

struct _IOFunc {
    int type;
    Symbol* symbol;
    int number;
    char* string;
};
typedef struct _IOFunc IOFunc;


struct _ControlFlow {
    int type;
    AST* cond;
    AST* then_block;
    AST* else_block; // NULL in while statement
};
typedef struct _ControlFlow ControlFlow;



// Constructors
AST* mk_ast(int type, AST* left, AST* right);
AST* mk_num(int num);
AST* mk_assign(Symbol* sym, AST* val);
AST* mk_sym(Symbol* sym);
AST* mk_flow(int type, AST* cond, AST* then_block, AST* else_block);
AST* mk_func(int type, Symbol* symbol, int number, char* string);



#endif



/*
  struct _IntExpr {
  enum { IntNum, IntOp } kind;
  union {
  int num;
  struct {
  enum { ADD, SUB, MULT, DIV, MOD } type;
  struct _IntExpr *left;
  struct _IntExpr *right;
  } op;
  } u;
  };
  typedef struct _IntExpr IntExpr;


  struct _BoolExpr {
  enum { BoolVal, BoolOp } kind;
  union {
  bool val;
  struct {
  enum { GRT, GEQ, LRT, LEQ, NEQ, EQT, NOT, AND, OR } type;
  struct _BoolExpr *left;
  struct _BoolExpr *right;
  } op;
  } u;
  };
  typedef struct _BoolExpr BoolExpr;
*/

