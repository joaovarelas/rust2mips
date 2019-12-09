#include <stdio.h>
#include <stdbool.h>
#include "parser.h"

char* compileExpr(AST* expr);
char* compileBool(AST* expr, char* label_true, char* label_false);

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


char* compileExpr(AST* expr){
    char* t = NULL;

    switch(type_map[expr->type]){

    case TRM:
        {
            t = tx();
            int val = (expr->type == SYM) ? ((SymbolRef*)expr)->sym->val : ((IntVal*)expr)->num;
            set_symbol_value(t, val);
            add_instr( mk_instr(ATRIB, mk_atom_str(t), mk_atom_int(val), mk_atom_empty(), mk_atom_empty()), list);
        }
        break;


    case ARI:
        {
            char* left = compileExpr(expr->left);
            char* right = compileExpr(expr->right);
            Type type;
            int val;
            switch(expr->type){
            case ADD:
                val = get_symbol_value(left) + get_symbol_value(right);
                type = PLUS;
                break;
            case SUB:
                val = get_symbol_value(left) - get_symbol_value(right);
                type = MINUS;
                break;
            case MULT:
                val = get_symbol_value(left) * get_symbol_value(right);
                type = MULTI;
                break;
            case DIV:
                val = get_symbol_value(left) / get_symbol_value(right);
                type = DIVI;
                break;
            }
            t = tx();
            set_symbol_value(t, val);
            add_instr( mk_instr(type, mk_atom_str(t), mk_atom_str(left), mk_atom_str(right), mk_atom_empty()), list);
        }
        break;


    case LOG:
        {
            char* l1 = lx();
            char* l2 = lx();
            char* t = tx();
            set_symbol_value(t, 0);
            add_instr( mk_instr(ATRIB, mk_atom_str(t), mk_atom_int(0), mk_atom_empty(), mk_atom_empty()), list);
            char* r = compileBool(expr, l1, l2);
            add_instr( mk_instr(LABEL, mk_atom_str(l1), mk_atom_empty(), mk_atom_empty(), mk_atom_empty()), list);
            set_symbol_value(t, 1);
            add_instr( mk_instr(ATRIB, mk_atom_str(t), mk_atom_int(1), mk_atom_empty(), mk_atom_empty()), list);
            add_instr( mk_instr(LABEL, mk_atom_str(l2), mk_atom_empty(), mk_atom_empty(), mk_atom_empty()), list);
        }
        break;

    default:
        printf("unknown case: compileExpr(), type_map[expr->type]\n");
        break;
    }

    return t;

}


char* compileBool(AST* expr, char* lab_t, char* lab_f){
    char* t = NULL;

    switch(type_map[expr->type]){
    case LOG:
        {
          // TODO: Refactor compileExpr usage
          char* left;
          char* right;
          char* l1;
          if (expr->type != AND && expr->type != OR) {
            left = compileExpr(expr->left);
            right = compileExpr(expr->right);
          }

            switch(expr->type){
            case EQT:
                add_instr( mk_instr(IFE, mk_atom_str(left), mk_atom_str(right), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
                break;
            case NEQ:
                add_instr( mk_instr(IFNE, mk_atom_str(left), mk_atom_str(right), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
                break;
            case GRT:
                add_instr( mk_instr(IFG, mk_atom_str(left), mk_atom_str(right), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
                break;
            case GEQ:
                add_instr( mk_instr(IFGE, mk_atom_str(left), mk_atom_str(right), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
                break;
            case LRT:
                add_instr( mk_instr(IFL, mk_atom_str(left), mk_atom_str(right), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
                break;
            case LEQ:
                add_instr( mk_instr(IFLE, mk_atom_str(left), mk_atom_str(right), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
                break;

            case AND:
                printf(""); // C is weird ... (Can't make a declaration directly after a label)
                l1 = lx();
                left = compileBool(expr->left, l1, lab_f);
                add_instr( mk_instr(LABEL, mk_atom_str(l1), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
                right = compileBool(expr->right, lab_t, lab_f);
                break;
            case OR:
                printf("");
                l1 = lx();
                left = compileBool(expr->left, lab_t, l1);
                add_instr( mk_instr(LABEL, mk_atom_str(l1), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
                right = compileBool(expr->right, lab_t, lab_f);
                break;
            case NOT:
                compileBool(expr->left, lab_f, lab_t);
                break;
            }
        }
    case TRM:
        {
            int val = (expr->type == SYM) ? ((SymbolRef*)expr)->sym->val : ((IntVal*)expr)->num;
            char* lab = ( val == 0 ) ? lab_f : lab_t;
            add_instr( mk_instr(GOTO, mk_atom_str(lab), mk_atom_empty(), mk_atom_empty(), mk_atom_empty()), list);
        }
        break;

    case ARI:
        {
            char* t = tx();
            char* r = compileExpr(expr);
            add_instr( mk_instr(IFNE, mk_atom_str(t), mk_atom_int(0), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
        }
        break;


    default:
        printf("unknown case: compileBool()\n");
        break;
    }

    return t;
}


void compileCmd(AST* node){
    if (node == NULL)
        return;


    switch (node->type) {
    case CMD:
        {
            compileCmd(node->left);
            compileCmd(node->right);
        }
        break;


    case ASG:
        {
            char* var = ((AssignVal*)node)->sym->name;
            AST* expr = ((AssignVal*)node)->val;
            char* r = compileExpr( expr );
            set_symbol_value(var, get_symbol_value(r));
            add_instr( mk_instr(ATRIB, mk_atom_str(var), mk_atom_str(r),  mk_atom_empty(), mk_atom_empty()), list);
        }
        break;

    case IFS:
        {
            char* l1 = lx();
            char* l2 = lx();
            char* t = compileBool( ((ControlFlow*)node)->cond , l1, l2);
            add_instr(mk_instr(LABEL, mk_atom_str(l1), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
            compileCmd( ((ControlFlow*)node)->then_block );
            add_instr(mk_instr(LABEL, mk_atom_str(l2), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
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
    print_list(list);

    return 0;
}
