#include <stdio.h>
#include <stdbool.h>
#include "parser.h"

InstrList* list;


// t0 - t999
int t_count = 0;
char* tx(){
    char* t = (char*)malloc(sizeof( 5 * sizeof(char) )); // floor(log10(n)) + 1 -> nr of digits of counter
    sprintf(t, "t%d", t_count++);
    return t;
}

// _L0 - _L999
int label_count = 0;
char* lx(){
    char* l = (char*)malloc(sizeof(6 * sizeof(char) ));
    sprintf(l, "_L%d", label_count++);
    return l;
}



// Append arithmetic instRuctions to list
// Returns temp. register which holds result (on a Symbol* hashmap)
char* compileExpr(AST* expr){
    char* t = NULL;

    AST* e_left = expr->left;
    AST* e_right = expr->right;
    int left_type = e_left->type;
    int right_type = e_right->type;
    
    switch(expr->type){
    case ADD:
	{
	    // int + int
	    if(type_map[left_type] == TRM && type_map[right_type] == TRM){
		int left = (left_type == SYM) ? ((SymbolRef*)e_left)->sym->val : ((IntVal*)e_left)->num;
		int right = (right_type == SYM) ? ((SymbolRef*)e_right)->sym->val : ((IntVal*)e_right)->num;
		t = tx();
		add_instr(mk_instr(PLUS, mk_atom_str(t), mk_atom_int(left), mk_atom_int(right)), list);
		set_symbol_value(t, left+right);
		printf("%s := %d + %d [ = %d ]\n", t, left, right, get_symbol_value(t));

		// int + expr
	    }else if(type_map[left_type] == TRM){
		int left = (left_type == SYM) ? ((SymbolRef*)e_left)->sym->val : ((IntVal*)e_left)->num;
		char* right = compileExpr(e_right);
		t = tx();
		add_instr( mk_instr(PLUS, mk_atom_str(t), mk_atom_int(left), mk_atom_str(right)), list);
		set_symbol_value(t, left+get_symbol_value(right));
		printf("%s := %d + %s [ = %d ]\n", t, left, right, get_symbol_value(t));

		// expr + int
	    }else if(type_map[right_type] == TRM){
		char* left = compileExpr(e_left);
		int right = ((IntVal*)e_right)->num;
		t = tx();
		add_instr(mk_instr(PLUS, mk_atom_str(t), mk_atom_str(left), mk_atom_int(right)), list);
		set_symbol_value(t, get_symbol_value(left)+right);
		printf("%s := %s + %d [ = %d ]\n", t, left, right, get_symbol_value(t));

		// expr + expr
	    }else{
		char* left = compileExpr(e_left);
		char* right = compileExpr(e_right);
		t = tx();
		add_instr(mk_instr(PLUS, mk_atom_str(t), mk_atom_str(left), mk_atom_str(right)), list);
		set_symbol_value(t, get_symbol_value(left) +  get_symbol_value(right));
		printf("%s := %s + %s [ = %d ]\n", t, left, right,  get_symbol_value(t));
	    }
	}
	break;


    case SUB:
	{
	    // int - int
	    if(type_map[left_type] == TRM && type_map[right_type] == TRM){
		int left = (left_type == SYM) ? ((SymbolRef*)e_left)->sym->val : ((IntVal*)e_left)->num;
		int right = (right_type == SYM) ? ((SymbolRef*)e_right)->sym->val : ((IntVal*)e_right)->num;
		t = tx();
		add_instr(mk_instr(MINUS, mk_atom_str(t), mk_atom_int(left), mk_atom_int(right)), list);
		set_symbol_value(t, left-right);
		printf("%s := %d - %d [ = %d ]\n", t, left, right, get_symbol_value(t));

		// int - expr
	    }else if(type_map[left_type] == TRM){
		int left = (left_type == SYM) ? ((SymbolRef*)e_left)->sym->val : ((IntVal*)e_left)->num;
		char* right = compileExpr(e_right);
		t = tx();
		add_instr( mk_instr(MINUS, mk_atom_str(t), mk_atom_int(left), mk_atom_str(right)), list);
		set_symbol_value(t, left-get_symbol_value(right));
		printf("%s := %d - %s [ = %d ]\n", t, left, right, get_symbol_value(t));

		// expr - int
	    }else if(type_map[right_type] == TRM){
		char* left = compileExpr(e_left);
		int right = (right_type == SYM) ? ((SymbolRef*)e_right)->sym->val : ((IntVal*)e_right)->num;
		t = tx();
		add_instr(mk_instr(MINUS, mk_atom_str(t), mk_atom_str(left), mk_atom_int(right)), list);
		set_symbol_value(t, get_symbol_value(left)-right);
		printf("%s := %s - %d [ = %d ]\n", t, left, right, get_symbol_value(t));

		// expr - expr
	    }else{
		char* left = compileExpr(e_left);
		char* right = compileExpr(e_right);
		t = tx();
		add_instr(mk_instr(MINUS, mk_atom_str(t), mk_atom_str(left), mk_atom_str(right)), list);
		set_symbol_value(t, get_symbol_value(left)-get_symbol_value(right));
		printf("%s := %s - %s [ = %d ]\n", t, left, right,  get_symbol_value(t));
	    }
	}
	break;


    case MULT:
	{
	    // int * int
	    if(type_map[left_type] == TRM && type_map[right_type] == TRM){
		int left = (left_type == SYM) ? ((SymbolRef*)e_left)->sym->val : ((IntVal*)e_left)->num;
		int right = (right_type == SYM) ? ((SymbolRef*)e_right)->sym->val : ((IntVal*)e_right)->num;
		t = tx();
		add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_int(left), mk_atom_int(right)), list);
		set_symbol_value(t, left*right);
		printf("%s := %d * %d [ = %d ]\n", t, left, right, get_symbol_value(t));

		// int * expr
	    }else if(type_map[left_type] == TRM){
		int left = (left_type == SYM) ? ((SymbolRef*)e_left)->sym->val : ((IntVal*)e_left)->num;
		char* right = compileExpr(e_right);
		t = tx();
		add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_int(left), mk_atom_str(right)), list);
		set_symbol_value(t, left*get_symbol_value(right));
		printf("%s := %d * %s [ = %d ]\n", t, left, right, get_symbol_value(t));

		// expr * int
	    }else if(type_map[right_type] == TRM){
		char* left = compileExpr(e_left);
		int right = (right_type == SYM) ? ((SymbolRef*)e_right)->sym->val : ((IntVal*)e_right)->num;
		t = tx();
		add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_str(left), mk_atom_int(right)), list);
		set_symbol_value(t, get_symbol_value(left)*right);
		printf("%s := %s * %d [ = %d ]\n", t, left, right, get_symbol_value(t));

		// expr * expr
	    }else{
		char* left = compileExpr(e_left);
		char* right = compileExpr(e_right);
		t = tx();
		add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_str(left), mk_atom_str(right)), list);
		set_symbol_value(t, get_symbol_value(left)*get_symbol_value(right));
		printf("%s := %s * %s [ = %d ]\n", t, left, right,  get_symbol_value(t));
	    }
	}
	break;

    case DIV:
	{
	    // int / int
	    if(type_map[left_type] == TRM && type_map[right_type] == TRM){
		int left = (left_type == SYM) ? ((SymbolRef*)e_left)->sym->val : ((IntVal*)e_left)->num;
		int right = (right_type == SYM) ? ((SymbolRef*)e_right)->sym->val : ((IntVal*)e_right)->num;
		t = tx();
		add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_int(left), mk_atom_int(right)), list);
		set_symbol_value(t, left/right);
		printf("%s := %d / %d [ = %d ]\n", t, left, right, get_symbol_value(t));

		// int / expr
	    }else if(type_map[left_type] == TRM){
		int left = (left_type == SYM) ? ((SymbolRef*)e_left)->sym->val : ((IntVal*)e_left)->num;
		char* right = compileExpr(e_right);
		t = tx();
		add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_int(left), mk_atom_str(right)), list);
		set_symbol_value(t, left/get_symbol_value(right));
		printf("%s := %d / %s [ = %d ]\n", t, left, right, get_symbol_value(t));

		// expr / int
	    }else if(type_map[right_type] == TRM){
		char* left = compileExpr(e_left);
		int right = (right_type == SYM) ? ((SymbolRef*)e_right)->sym->val : ((IntVal*)e_right)->num;
		t = tx();
		add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_str(left), mk_atom_int(right)), list);
		set_symbol_value(t, get_symbol_value(left)/right);
		printf("%s := %s / %d [ = %d ]\n", t, left, right, get_symbol_value(t));

		// expr / expr
	    }else{
		char* left = compileExpr(e_left);
		char* right = compileExpr(e_right);
		t = tx();
		add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_str(left), mk_atom_str(right)), list);
		set_symbol_value(t, get_symbol_value(left)/get_symbol_value(right));
		printf("%s := %s / %s [ = %d ]\n", t, left, right,  get_symbol_value(t));
	    }
	}
	break;

    default:
	printf("unknown case: compileExpr()\n");
	break;
    }

    return t;

}


// 	GRT, GEQ, LRT, LEQ, EQT, NEQ, AND, OR, NOT,
char* compileBool(AST* expr){
    char* t = NULL;
    
    AST* e_left = expr->left;
    AST* e_right = expr->right;
    int left_type = e_left->type;
    int right_type = e_right->type;
    
    switch(expr->type){
    case GRT:
	{	    
	}
	break;
    case GEQ:
	{
	}
	break;
    case LRT:
	{
	}
	break;
    case LEQ:
	{
	}
	break;
    case EQT:
	{
	}
	break;
    case NEQ:
	{
	}
	break;
    case AND:
	{
	    // bool && bool
	    if(type_map[left_type] == TRM && type_map[right_type] == TRM){
		int left = (left_type == SYM) ? ((SymbolRef*)e_left)->sym->val : ((IntVal*)e_left)->num;
		int right = (right_type == SYM) ? ((SymbolRef*)e_right)->sym->val : ((IntVal*)e_right)->num;
		t = tx();
		//add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_int(left), mk_atom_int(right)), list);
		set_symbol_value(t, left && right);
		printf("%s := %d && %d [ = %d ]\n", t, left, right, get_symbol_value(t));

		// bool && expr
	    }else if(type_map[left_type] == TRM){
		int left = (left_type == SYM) ? ((SymbolRef*)e_left)->sym->val : ((IntVal*)e_left)->num;
		char* right = (type_map[right_type] == LOG ) ? compileBool(e_right) : compileExpr(e_right);
		t = tx();
		//add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_int(left), mk_atom_str(right)), list);
		set_symbol_value(t, left && get_symbol_value(right));
		printf("%s := %d && %s [ = %d ]\n", t, left, right, get_symbol_value(t));

		// expr && bool
	    }else if(type_map[right_type] == TRM){
		char* left = (type_map[left_type] == LOG ) ? compileBool(e_left) : compileExpr(e_left);
		int right = (right_type == SYM) ? ((SymbolRef*)e_right)->sym->val : ((IntVal*)e_right)->num;
		t = tx();
		//add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_str(left), mk_atom_int(right)), list);
		set_symbol_value(t, get_symbol_value(left) && right);
		printf("%s := %s && %d [ = %d ]\n", t, left, right, get_symbol_value(t));

		// expr && expr
	    }else{
		char* left = (type_map[left_type] == LOG ) ? compileBool(e_left) : compileExpr(e_left);
		char* right = (type_map[right_type] == LOG ) ? compileBool(e_right) : compileExpr(e_right);
		t = tx();
		//add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_str(left), mk_atom_str(right)), list);
		set_symbol_value(t, get_symbol_value(left) && get_symbol_value(right));
		printf("%s := %s && %s [ = %d ]\n", t, left, right,  get_symbol_value(t));
	    }
	}
	break;
    case OR:
	{
	}
	break;
    case NOT:
	{
	}
	break;
	
    default:
	printf("unknown case: compileBool()\n");
	break;
    }

    return t;
}


void compileCmd(AST* node){
    if(node == NULL)
	return;

    switch(node->type){
    case CMD:
	compileCmd(node->left);
        compileCmd(node->right);
	break;
	
	// ARI, LOG, NUM
    case ASG:
	{
	    char* r = ((AssignVal*)node)->sym->name;
	    AST* expr = ((AssignVal*)node)->val;
	    int e_type = expr->type;
	    
	    if(type_map[e_type] == ARI){ 
		char* t = compileExpr(expr);
		add_instr(mk_instr(ATRIB, mk_atom_str(r), mk_atom_str(t), mk_atom_empty()), list);
		Symbol* s = search_table(t);
		printf("%s := %s [ = %d ]\n", r, t, s->val);
		set_symbol_value(r, s->val);
		//printf("%s := %d\n", r, s->val);
	    }else if(type_map[e_type] == LOG){
		char* t = compileBool(expr);
		add_instr(mk_instr(ATRIB, mk_atom_str(r), mk_atom_str(t), mk_atom_empty()), list);
		Symbol* s = search_table(t);
		printf("%s := %s [ = %d ]\n", r, t, s->val);
		set_symbol_value(r, s->val);
	    }else if(type_map[e_type] == TRM){
		char* t = tx();
		add_instr(mk_instr(ATRIB, mk_atom_str(r), mk_atom_str(t), mk_atom_empty()), list);
		int val = (expr->type == SYM) ? ((SymbolRef*)expr)->sym->val : ((IntVal*)expr)->num;
       		Symbol* s = search_table(t);
		set_symbol_value(t, val);
		printf("%s := %d [ = %d ]\n", t, val, get_symbol_value(t));
		printf("%s := %s [ = %d ]\n", r, t, val);
		set_symbol_value(r, val);
	    }else{
		printf("unknown type: compileCmd(), case ASG\n");
	    }
	}
	break;

    case IFS:
	{
	    AST* expr_cond = ((ControlFlow*)node)->cond;
	    AST* then_cmd = ((ControlFlow*)node)->then_block;
	    
	}
	break;

    case WHS:
	{
	}
	break;

    case PTL:
	{
	}
	break;

    case RDL:
	{
	}
	break;

    default:
	printf("unknown case: compileCmd()\n");
	break;
    }

    return;
}


int main(int argc, char** argv) {
    --argc; ++argv;


    // Initialize instruction list with "main" label
    list = mk_instr_list( mk_instr(LABEL, mk_atom_str("main"), mk_atom_empty(), mk_atom_empty()), NULL);

    printf("\n====================\n");
    if (argc != 0) {
	yyin = fopen(*argv, "r");
	if (!yyin) {
	    printf("'%s': could not open file\n", *argv);
	    return 1;
	}
    }

    // print source code
    char c;
    FILE* file = fopen(*argv, "r");
    if (file) {
	while ( ( c = getc(file)) != EOF)
	    putchar(c);
	fclose(file);
    }
   
 
    //  yyin = stdin
    if (yyparse() == 0) {
	printf("====================\n");
	compileCmd(root);
    }
    
    printf("====================\n");
    print_list(list);

    return 0;
}
