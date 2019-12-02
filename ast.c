// AST constructor functions
#include <stdio.h>
#include <stdlib.h> // for malloc
#include <string.h>
#include "ast.h" // AST header



AST* mk_ast(int type, AST* left, AST* right){
    AST* ast = (AST*)malloc(sizeof(AST));
    ast->type = type;
    ast->left = left;
    ast->right = right;
    return ast;
}

AST* mk_num(int number){
    IntVal* intval = (IntVal*)malloc(sizeof(IntVal));
    intval->type = NUM;
    intval->num = number;
    return (AST*)intval;
}


AST* mk_sym(Symbol* sym){
    SymbolRef* symr = (SymbolRef*)malloc(sizeof(SymbolRef));
    symr->type = SYM;
    symr->sym = sym;
    return (AST*)symr;
}

AST* mk_assign(Symbol* sym, AST* val){
    AssignVal* asg = (AssignVal*)malloc(sizeof(AssignVal));
    asg->type = ASG;
    asg->sym = sym;
    asg->val = val;
    return (AST*)asg;
}

AST* mk_func(int type, Symbol* symbol, int number, char* string){
    IOFunc* fn = (IOFunc*)malloc(sizeof(IOFunc));
    fn->type = type;
    fn->symbol = symbol;
    fn->number = number;
    fn->string = string;
    return (AST*)fn;
}

AST* mk_flow(int type, AST* cond, AST* then_block, AST* else_block){
    ControlFlow* flow = (ControlFlow*)malloc(sizeof(ControlFlow));
    flow->type = type;
    flow->cond = cond;
    flow->then_block = then_block;
    flow->else_block = else_block;
    return (AST*)flow;
}




unsigned int mk_hash(char* var){
    unsigned int c = 0,
	hashval = 0;
    while(c++ < strlen(var)) hashval += var[c];
    hashval += strlen(var);
    //printf("hash(%s) = %u\n", var, hashval);
    return hashval;
}

Symbol* search_table(char* var){
    int k = MAX_ENTRIES;
    Symbol* sp = &hash_table[ mk_hash(var) % k];

    while(--k >= 0) {
	if(sp->name && !strcmp(sp->name, var))
	    return sp;

	if(!sp->name) {
	    sp->name = strdup(var);
	    sp->val = 0;
	    return sp;
	}

	/* new entry */
	if(++sp >= hash_table + MAX_ENTRIES)
	    sp = hash_table; /* try the next entry */
    }

    yyerror("symbol table overflow\n");
    abort(); /* tried them all, table is full */
}
