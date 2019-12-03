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
    unsigned int hash = 0, c=0;
   
    while( var[c] ){
	hash += var[c];
        hash += (hash << 10);
        hash ^= (hash >> 6);
	++c;
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash -= (hash << 15);

    hash %= MAX_ENTRIES;
    //printf("Debug: hash(%s) = %u\n", var, hash);
    return hash;
}


Symbol* search_table(char* var){
    int scount = MAX_ENTRIES;
    Symbol* sp = &hash_table[ mk_hash(var) ];
        
    while(--scount >= 0) {
	if(sp->name && !strcmp(sp->name, var)) { return sp; }

	if(!sp->name) {		/* new entry */
	    sp->name = strdup(var);
	    sp->val = 0;
	    return sp;
	}
	
	if(++sp >= hash_table + MAX_ENTRIES)
	    sp = hash_table; /* try the next entry */
    }
    yyerror("symbol table overflow\n");
    abort(); /* tried them all, table is full */

}


void set_symbol_value(char* var, int value){
    unsigned int hash = mk_hash(var);
    Symbol* s = &hash_table[ hash ];
    s->name = strdup(var);
    s->val = value;
    return;
}

int get_symbol_value(char* var){
    return (search_table(var))->val;
}

