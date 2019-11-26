// AST definitions
#ifndef __ast_h__
#define __ast_h__

#include <stdbool.h>

// hashtable max entries (prime number to reduce possible collisions)
#define MAX_ENTRIES 1009

extern void yyerror(const char* msg);


enum _NodeType {
		/* NUM (int and bool as 0, 1)) */
		NUM, SYM,

		/* OPR (arithmetic) */
		ADD, SUB, MULT, DIV, MOD,

		/* CMP (comparison) */
		GRT, GEQ, LRT, LEQ, EQT, NEQ,

		/* LOG (logical operators)*/
		NOT, AND, OR,

		/* ASG (assignment)*/
		ASG,

		/* FLW (if-then-else / while-do flow), LST (list of commands)*/
		IFS, WHS, CMD,

		/* IO funcs (println!, read_line) */
		PTL, RDL

};
typedef enum _NodeType Type;


// Tree Struct
struct _AST {
    int type;
    struct _AST* left;
    struct _AST* right;
};
typedef struct _AST AST;



struct _IntExpr {
    enum { IntNum, IntOp} kind;
    union {
	int num;
	struct {
	    Type type;
	    struct _IntExpr *left;
	    struct _IntExpr *right;
	} op;
    } u;
};
typedef struct _IntExpr IntExpr;


struct _BoolExpr {
    enum { BoolVal, BoolOp} kind;
    union {
	bool val;
	struct {
	    Type type;
	    struct _BoolExpr *left;
	    struct _BoolExpr *right;
	} op;
    } u;
};
typedef struct _BoolExpr BoolExpr;





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


// Build Symbols Table (Hashtable)
Symbol hash_table[MAX_ENTRIES];
unsigned int mk_hash(char *var);
Symbol* search_table(char* var, int len);


#endif

