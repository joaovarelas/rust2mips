#include <stdio.h>
#include <stdbool.h>
#include "parser.h"

InstrList* list;


// t0 - t999 [ temporary symbols ]
int t_count = 0;
char* tx(){
    char* t = (char*)malloc(sizeof(5 * sizeof(char) ));
    sprintf(t, "t%d", t_count++);
    return t;
}



/*

  TO COMPLETE

*/

char* compileExpr(AST* expr, char* r){
    char* t = tx();

    switch(expr->type){
    case ADD:
	{
	    // int + int
	    if(expr->left->type == NUM && expr->right->type == NUM){
		int left = ((IntVal*)expr->left)->num;
		int right = ((IntVal*)expr->right)->num;
		Instr* i = mk_instr(PLUS, mk_atom_str(t), mk_atom_int(left), mk_atom_int(right));
		add_instr(i, list);
		printf("%s := %d + %d\n", t, left, right);

		// int + expr
	    }else if(expr->left->type == NUM){
		char* tt = compileExpr(expr->right, r);
		int left = ((IntVal*)expr->left)->num;
		Instr* i = mk_instr(PLUS, mk_atom_str(t), mk_atom_int(left), mk_atom_str(tt));
		add_instr(i, list);
		printf("%s := %d + %s\n", t, ((IntVal*)expr->left)->num, tt);
		
		// expr + int
	    }else if(expr->right->type == NUM){
		char* tt = compileExpr(expr->left, r);
		int right = ((IntVal*)expr->right)->num;
		Instr* i = mk_instr(PLUS, mk_atom_str(t), mk_atom_str(tt), mk_atom_int(right));
		add_instr(i, list);
		printf("%s := %s + %d\n", t, tt, ((IntVal*)expr->right)->num);
		
		// expr + expr
	    }else{
		char* tt0 = compileExpr(expr->left, r);
		char* tt1 = compileExpr(expr->right, r);
		Instr* i = mk_instr(PLUS, mk_atom_str(t), mk_atom_str(tt0), mk_atom_str(tt1));
		add_instr(i, list);
		printf("%s := %s + %s\n", t, tt0, tt1);
	    }

	}
	break;

	
    case SUB:
	{
	    // int + int
	    if(expr->left->type == NUM && expr->right->type == NUM){
		int left = ((IntVal*)expr->left)->num;
		int right = ((IntVal*)expr->right)->num;
		Instr* i = mk_instr(MINUS, mk_atom_str(t), mk_atom_int(left), mk_atom_int(right));
		add_instr(i, list);
		printf("%s := %d - %d\n", t, left, right);

		// int + expr
	    }else if(expr->left->type == NUM){
		char* tt = compileExpr(expr->right, r);
		int left = ((IntVal*)expr->left)->num;
		Instr* i = mk_instr(MINUS, mk_atom_str(t), mk_atom_int(left), mk_atom_str(tt));
		add_instr(i, list);
		printf("%s := %d - %s\n", t, ((IntVal*)expr->left)->num, tt);
		
		// expr + int
	    }else if(expr->right->type == NUM){
		char* tt = compileExpr(expr->left, r);
		int right = ((IntVal*)expr->right)->num;
		Instr* i = mk_instr(MINUS, mk_atom_str(t), mk_atom_str(tt), mk_atom_int(right));
		add_instr(i, list);
		printf("%s := %s - %d\n", t, tt, ((IntVal*)expr->right)->num);
		
		// expr + expr
	    }else{
		char* tt0 = compileExpr(expr->left, r);
		char* tt1 = compileExpr(expr->right, r);
		Instr* i = mk_instr(MINUS, mk_atom_str(t), mk_atom_str(tt0), mk_atom_str(tt1));
		add_instr(i, list);
		printf("%s := %s - %s\n", t, tt0, tt1);
	    }

	}
	break;

	
    case MULT:
	{
	    // int * int
	    if(expr->left->type == NUM && expr->right->type == NUM){
		int left = ((IntVal*)expr->left)->num;
		int right = ((IntVal*)expr->right)->num;
		Instr* i = mk_instr(MULTI, mk_atom_str(t), mk_atom_int(left), mk_atom_int(right));
		add_instr(i, list);
		printf("%s := %d * %d\n", t, left, right);

		// int * expr
	    }else if(expr->left->type == NUM){
		char* tt = compileExpr(expr->right, r);
		int left = ((IntVal*)expr->left)->num;
		Instr* i = mk_instr(MULTI, mk_atom_str(t), mk_atom_int(left), mk_atom_str(tt));
		add_instr(i, list);
		printf("%s := %d * %s\n", t, ((IntVal*)expr->left)->num, tt);
		
		// expr * int
	    }else if(expr->right->type == NUM){
		char* tt = compileExpr(expr->left, r);
		int right = ((IntVal*)expr->right)->num;
		Instr* i = mk_instr(MULTI, mk_atom_str(t), mk_atom_str(tt), mk_atom_int(right));
		add_instr(i, list);
		printf("%s := %s * %d\n", t, tt, ((IntVal*)expr->right)->num);
		
		// expr * expr
	    }else{
		char* tt0 = compileExpr(expr->left, r);
		char* tt1 = compileExpr(expr->right, r);
		Instr* i = mk_instr(MULTI, mk_atom_str(t), mk_atom_str(tt0), mk_atom_str(tt1));
		add_instr(i, list);
		printf("%s := %s * %s\n", t, tt0, tt1);
	    }
	}
	break;
	
	
    default:
	break;
    }

    return t;

}





void tree(AST* node){
    if(node == NULL)
	return;

    
    switch(node->type){
    case CMD:
	tree(node->left);
	tree(node->right);
	break;

    case ASG:
	{
	    char* r = ((AssignVal*)node)->sym->name;
	    AST* expr = ((AssignVal*)node)->val;
	    char* x = compileExpr(expr, r);
	    Instr* i = mk_instr(ATRIB, mk_atom_str(r), mk_atom_str(x), mk_atom_empty());
	    add_instr(i, list);
	    printf("%s := %s\n", r, x);
	}
	break;

    case IFS:
	{
	}
	break;

    case WHS:
	break;

    case PTL:
	break;

    case RDL:
	break;
	
    default:
	break;
    }
    
    return;
}


int main(int argc, char** argv) {
    --argc; ++argv;

  
    Instr* i = mk_instr(LABEL, mk_atom_str("main"), mk_atom_empty(), mk_atom_empty());
    list = mk_instr_list(i, NULL);

    printf("\n====================\n");
    if (argc != 0) {
	yyin = fopen(*argv, "r");
	if (!yyin) {
	    printf("'%s': could not open file\n", *argv);
	    return 1;
	}
    }
     
    //  yyin = stdin
    if (yyparse() == 0) { 
	tree(root);
    }
    printf("====================\n");

    print_list(list);
    
    return 0;
}

