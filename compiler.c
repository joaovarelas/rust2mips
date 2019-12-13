#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "parser.h"

// Prototypes
void compileExpr(AST* expr, char* t);
void compileBool(AST* expr, char* label_true, char* label_false);

InstrList* list;

int ndigits(int x){
    return (x == 0) ? 1 : floor(log(x))+1 ;
}

// t0 - t999
int t_count = 0;
char* tx(){
    char* t = (char*)malloc(sizeof( ndigits(t_count) * sizeof(char) ));
    sprintf(t, "$t%d", t_count++);
    return t;
}

// _L0 - _L999
int label_count = 0;
char* lx(){
    char* l = (char*)malloc( sizeof(ndigits(label_count) * sizeof(char) ));
    sprintf(l, "_L%d", label_count++);
    return l;
}


void compileExpr(AST* expr, char* t){
    //char* t = NULL;

    switch(type_map[expr->type]){

    case TRM:
        {
            //t = tx();

            switch(expr->type){
            case NUM:
                {
                    int val = ((IntVal*)expr)->num;
                    set_symbol_value(t, val);
                    add_instr( mk_instr(ATRIB, mk_atom_str(t), mk_atom_int(val), mk_atom_empty(), mk_atom_empty()), list);
                }
                break;
            case SYM:
                {
                    char* val = ((SymbolRef*)expr)->sym->name;
                    set_symbol_value(t, ((SymbolRef*)expr)->sym->val);
                    add_instr( mk_instr(ATRIB, mk_atom_str(t), mk_atom_str(val), mk_atom_empty(), mk_atom_empty()), list);
                }
                break;
            }
        }
        break;


    case ARI:
        {
            char* t1 = tx();
            char* t2 = tx();
            compileExpr(expr->left, t1);
            compileExpr(expr->right, t2);
            Type type;
            int val;

            switch(expr->type){
            case ADD:
                {
                    val = get_symbol_value(t1) + get_symbol_value(t2);
                    type = PLUS;
                }
                break;
            case SUB:
                {
                    val = get_symbol_value(t1) - get_symbol_value(t2);
                    type = MINUS;
                }
                break;
            case MULT:
                {
                    val = get_symbol_value(t1) * get_symbol_value(t2);
                    type = MULTI;
                }
                break;
            case DIV:
                {
                    val = get_symbol_value(t1) / get_symbol_value(t2);
                    type = DIVI;
                }
                break;
            }
            set_symbol_value(t, val);
            add_instr( mk_instr(type, mk_atom_str(t), mk_atom_str(t1), mk_atom_str(t2), mk_atom_empty()), list);
        }
        break;


    default:
        printf("unknown case: compileExpr(), type_map[expr->type]\n");
        break;
    }

    return; //t;

}


void compileBool(AST* expr, char* lab_t, char* lab_f){

    switch(type_map[expr->type]){
    case REL:
        {

            char* t1 = tx();
            compileExpr(expr->left, t1);
            char* t2 = tx();
            compileExpr(expr->right, t2);

            switch(expr->type){
            case EQT:
                add_instr( mk_instr(IFE, mk_atom_str(t1), mk_atom_str(t2), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
                break;
            case NEQ:
                add_instr( mk_instr(IFNE, mk_atom_str(t1), mk_atom_str(t2), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
                break;
            case GRT:
                add_instr( mk_instr(IFG, mk_atom_str(t1), mk_atom_str(t2), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
                break;
            case GEQ:
                add_instr( mk_instr(IFGE, mk_atom_str(t1), mk_atom_str(t2), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
                break;
            case LRT:
                add_instr( mk_instr(IFL, mk_atom_str(t1), mk_atom_str(t2), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
                break;
            case LEQ:
                add_instr( mk_instr(IFLE, mk_atom_str(t1), mk_atom_str(t2), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
                break;
            }

        }
        /* falta verificar p variaveis (TRM->SYM) true false */
    case NUM:
        {
            int val = ((IntVal*)expr)->num;
            if( val == 1 )
                add_instr(mk_instr(GOTO, mk_atom_str(lab_t), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
            else if( val == 0 )
                add_instr(mk_instr(GOTO, mk_atom_str(lab_f), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
        }
        break;

    case LOG:
        {
            char* arg2 = lx();
            switch(expr->type){
            case AND:
                compileBool(expr->left, arg2, lab_f);
                break;

            case OR:
                compileBool(expr->left, lab_t, arg2);
                break;
            }
            add_instr( mk_instr(LABEL, mk_atom_str(arg2), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
            compileBool(expr->right, lab_t, lab_f);
        }
        break;

    case ARI:
        {
            char* t = tx();
            compileExpr(expr, t);
            add_instr( mk_instr(IFNE, mk_atom_str(t), mk_atom_int(0),  mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
        }
        break;

    default:
        printf("unknown case: compileBool()\n");
        break;
    }

    return;
}


void compileCmd(AST* cmd){
    if (cmd == NULL)
        return;


    switch (cmd->type) {
    case CMD:
        {
            compileCmd(cmd->left);
            compileCmd(cmd->right);
        }
        break;


    case ASG:
        {
            char* var = ((AssignVal*)cmd)->sym->name;
            // Symbol* s = search_table(var); // verificar
            char* r = tx();
            AST* expr = ((AssignVal*)cmd)->val;
            compileExpr( expr, r );
            set_symbol_value(var, get_symbol_value(r));
            add_instr( mk_instr(ATRIB, mk_atom_str(var), mk_atom_str(r),  mk_atom_empty(), mk_atom_empty()), list);
        }
        break;

    case IFS:
        {

            ControlFlow* ifcmd = (ControlFlow*)cmd;
            char* l1 = lx();
            char* l2 = lx();
            compileBool( ifcmd->cond , l1, l2);
            add_instr(mk_instr(LABEL, mk_atom_str(l1), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
            compileCmd( ifcmd->then_block );

            // IF THEN
            if(ifcmd->else_block == NULL){
                add_instr(mk_instr(LABEL, mk_atom_str(l2), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);

                // IF THEN ELSE
            }else{
                char* l3 = lx();
                add_instr(mk_instr(GOTO, mk_atom_str(l3), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
                add_instr(mk_instr(LABEL, mk_atom_str(l2), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
                compileCmd( ifcmd->else_block );
                add_instr(mk_instr(LABEL, mk_atom_str(l3), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
            }

        }
        break;
    case WHS:
        {
            char* l1 = lx();
            char* l2 = lx();
            char* l3 = lx();
            add_instr(mk_instr(LABEL, mk_atom_str(l1), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
            compileBool(((ControlFlow*)cmd)->cond, l2, l3);
            add_instr(mk_instr(LABEL, mk_atom_str(l2), mk_atom_empty(), mk_atom_empty(), mk_atom_empty()), list);
            compileCmd(((ControlFlow*)cmd)->then_block);
            add_instr(mk_instr(GOTO, mk_atom_str(l1), mk_atom_empty(), mk_atom_empty(), mk_atom_empty()), list);
            add_instr(mk_instr(LABEL, mk_atom_str(l3), mk_atom_empty(), mk_atom_empty(), mk_atom_empty()), list);
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
    list = mk_instr_list( mk_instr(LABEL, mk_atom_str("_main"), mk_atom_empty(), mk_atom_empty(), mk_atom_empty()), NULL);

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
    print_3AC(list);
    printf("====================\n");
    print_MIPS(list);


    return 0;
}
