#include <stdio.h>
#include <stdbool.h>
#include "parser.h"

InstrList* list;


// t0 - t999
int t_count = 0;
char* tx(){
    char* t = (char*)malloc(sizeof(5 * sizeof(char) ));
    sprintf(t, "t%d", t_count++);
    return t;
}

int label_count = 0;
char* lx(){
    char* l = (char*)malloc(sizeof(10 * sizeof(char) ));
    sprintf(l, "LABEL%d", label_count++);
    return l;
}



// Append arithmetic instructions to list
// Returns temp. register which holds result (on a Symbol* hashmap)
char* compileExpr(AST* expr){
    char* t = NULL;

    switch(expr->type){
    case ADD:
        {
            // int + int
            if(expr->left->type == NUM && expr->right->type == NUM){
                int left = ((IntVal*)expr->left)->num;
                int right = ((IntVal*)expr->right)->num;
                t = tx();
                add_instr(mk_instr(PLUS, mk_atom_str(t), mk_atom_int(left), mk_atom_int(right)), list);
                set_symbol_value(t, left+right);
                printf("%s := %d + %d [ = %d ]\n", t, left, right, get_symbol_value(t));

                // int + expr
            }else if(expr->left->type == NUM){
                int left = ((IntVal*)expr->left)->num;
                char* right = compileExpr(expr->right);
                t = tx();
                add_instr( mk_instr(PLUS, mk_atom_str(t), mk_atom_int(left), mk_atom_str(right)), list);
                set_symbol_value(t, left+get_symbol_value(right));
                printf("%s := %d + %s [ = %d ]\n", t, left, right, get_symbol_value(t));

                // expr + int
            }else if(expr->right->type == NUM){
                char* left = compileExpr(expr->left);
                int right = ((IntVal*)expr->right)->num;
                t = tx();
                add_instr(mk_instr(PLUS, mk_atom_str(t), mk_atom_str(left), mk_atom_int(right)), list);
                set_symbol_value(t, get_symbol_value(left)+right);
                printf("%s := %s + %d [ = %d ]\n", t, left, right, get_symbol_value(t));

                // expr + expr
            }else{
                char* left = compileExpr(expr->left);
                char* right = compileExpr(expr->right);
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
            if(expr->left->type == NUM && expr->right->type == NUM){
                int left = ((IntVal*)expr->left)->num;
                int right = ((IntVal*)expr->right)->num;
                t = tx();
                add_instr(mk_instr(MINUS, mk_atom_str(t), mk_atom_int(left), mk_atom_int(right)), list);
                set_symbol_value(t, left-right);
                printf("%s := %d - %d [ = %d ]\n", t, left, right, get_symbol_value(t));

                // int - expr
            }else if(expr->left->type == NUM){
                int left = ((IntVal*)expr->left)->num;
                char* right = compileExpr(expr->right);
                t = tx();
                add_instr( mk_instr(MINUS, mk_atom_str(t), mk_atom_int(left), mk_atom_str(right)), list);
                set_symbol_value(t, left-get_symbol_value(right));
                printf("%s := %d - %s [ = %d ]\n", t, left, right, get_symbol_value(t));

                // expr - int
            }else if(expr->right->type == NUM){
                char* left = compileExpr(expr->left);
                int right = ((IntVal*)expr->right)->num;
                t = tx();
                add_instr(mk_instr(MINUS, mk_atom_str(t), mk_atom_str(left), mk_atom_int(right)), list);
                set_symbol_value(t, get_symbol_value(left)-right);
                printf("%s := %s - %d [ = %d ]\n", t, left, right, get_symbol_value(t));

                // expr - expr
            }else{
                char* left = compileExpr(expr->left);
                char* right = compileExpr(expr->right);
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
            if(expr->left->type == NUM && expr->right->type == NUM){
                int left = ((IntVal*)expr->left)->num;
                int right = ((IntVal*)expr->right)->num;
                t = tx();
                add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_int(left), mk_atom_int(right)), list);
                set_symbol_value(t, left*right);
                printf("%s := %d * %d [ = %d ]\n", t, left, right, get_symbol_value(t));

                // int * expr
            }else if(expr->left->type == NUM){
                int left = ((IntVal*)expr->left)->num;
                char* right = compileExpr(expr->right);
                t = tx();
                add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_int(left), mk_atom_str(right)), list);
                set_symbol_value(t, left*get_symbol_value(right));
                printf("%s := %d * %s [ = %d ]\n", t, left, right, get_symbol_value(t));

                // expr * int
            }else if(expr->right->type == NUM){
                char* left = compileExpr(expr->left);
                int right = ((IntVal*)expr->right)->num;
                t = tx();
                add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_str(left), mk_atom_int(right)), list);
                set_symbol_value(t, get_symbol_value(left)*right);
                printf("%s := %s * %d [ = %d ]\n", t, left, right, get_symbol_value(t));

                // expr * expr
            }else{
                char* left = compileExpr(expr->left);
                char* right = compileExpr(expr->right);
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
            if(expr->left->type == NUM && expr->right->type == NUM){
                int left = ((IntVal*)expr->left)->num;
                int right = ((IntVal*)expr->right)->num;
                t = tx();
                add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_int(left), mk_atom_int(right)), list);
                set_symbol_value(t, left/right);
                printf("%s := %d / %d [ = %d ]\n", t, left, right, get_symbol_value(t));

                // int / expr
            }else if(expr->left->type == NUM){
                int left = ((IntVal*)expr->left)->num;
                char* right = compileExpr(expr->right);
                t = tx();
                add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_int(left), mk_atom_str(right)), list);
                set_symbol_value(t, left/get_symbol_value(right));
                printf("%s := %d / %s [ = %d ]\n", t, left, right, get_symbol_value(t));

                // expr / int
            }else if(expr->right->type == NUM){
                char* left = compileExpr(expr->left);
                int right = ((IntVal*)expr->right)->num;
                t = tx();
                add_instr(mk_instr(MULTI, mk_atom_str(t), mk_atom_str(left), mk_atom_int(right)), list);
                set_symbol_value(t, get_symbol_value(left)/right);
                printf("%s := %s / %d [ = %d ]\n", t, left, right, get_symbol_value(t));

                // expr / expr
            }else{
                char* left = compileExpr(expr->left);
                char* right = compileExpr(expr->right);
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
            t_count = 0; // reset temp var counter
            char* r = ((AssignVal*)node)->sym->name;
            AST* expr = ((AssignVal*)node)->val;
            char* t = compileExpr(expr);
            Instr* i = mk_instr(ATRIB, mk_atom_str(r), mk_atom_str(t), mk_atom_empty());
            add_instr(i, list);
            Symbol* s = search_table(t);
            printf("%s := %s [ = %d ]\n", r, t, s->val);
            set_symbol_value(r, s->val);
            printf("register %s = %d\n", r, s->val);
        }
        break;

    case IFS:
        {
            //Instr* i = mk_instr(IF, COND, TRUE, FALSE);
      
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
        printf("unknown case: tree()\n");
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

    //  yyin = stdin
    if (yyparse() == 0) {
        tree(root);
    }
    printf("====================\n");

    print_list(list);

    return 0;
}
