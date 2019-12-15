#include "parser.h"
#include "code.h"
#include "registers.h"
#include "interpreter.h"


// Global Instr List
InstrList* list;


// Prototypes
void compileExpr(AST* expr, char* t);
void compileBool(AST* expr, char* label_true, char* label_false);



void compileExpr(AST* expr, char* t){

    switch(type_map[expr->type]){
    case TRM:
        {
            //t = tx();

            switch(expr->type){
            case NUM:
                {
                    int val = ((IntVal*)expr)->num;
                    set_symbol(t, val);
                    add_instr( mk_instr(ATRIB, mk_atom_str(t), mk_atom_int(val), mk_atom_empty(), mk_atom_empty()), list);
                }
                break;
            case SYM:
                {
                    Symbol* s = ((SymbolRef*)expr)->sym;
                    set_symbol(t, s->val);
                    char* r = get_register(s->name);
                    add_instr( mk_instr(ATRIB, mk_atom_str(t), mk_atom_str(r), mk_atom_empty(), mk_atom_empty()), list);
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
                    val = get_symbol(t1) + get_symbol(t2);
                    type = PLUS;
                }
                break;
            case SUB:
                {
                    val = get_symbol(t1) - get_symbol(t2);
                    type = MINUS;
                }
                break;
            case MULT:
                {
                    val = get_symbol(t1) * get_symbol(t2);
                    type = MULTI;
                }
                break;
            case DIV:
                {
                    val = get_symbol(t1) / get_symbol(t2);
                    type = DIVI;
                }
                break;
            case MOD:
                {
                    val = get_symbol(t1) % get_symbol(t2);
                    type = MODUL;
                }
                break;
            }
            set_symbol(t, val);
            add_instr( mk_instr(type, mk_atom_str(t), mk_atom_str(t1), mk_atom_str(t2), mk_atom_empty()), list);
        }
        break;


    default:
        printf("unknown case: compileExpr(), type_map[expr->type] (%d)\n", expr->type);
        break;
    }

    return; //t;

}


void compileBool(AST* expr, char* lab_t, char* lab_f){

    switch(type_map[expr->type]){
    case REL:
        {

            char* t1 = tx();
            char* t2 = tx();
            compileExpr(expr->left, t1);
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


    case TRM:
        {
            if (expr->type == NUM && ((IntVal*)expr)->num != 0) {
              add_instr(mk_instr(GOTO, mk_atom_str(lab_t), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
            } else if (expr->type == SYM) {
              Symbol* s = ((SymbolRef*)expr)->sym;
              char* t = tx();
              add_instr(mk_instr(ATRIB, mk_atom_str(t), mk_atom_int(0), mk_atom_empty(), mk_atom_empty()), list);
              add_instr(mk_instr(IFNE, mk_atom_str( get_register(s->name)), mk_atom_str(t), mk_atom_str(lab_t), mk_atom_str(lab_f)), list);
              add_instr(mk_instr(GOTO, mk_atom_str(lab_f), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
            } else {
              add_instr(mk_instr(GOTO, mk_atom_str(lab_f), mk_atom_empty(),  mk_atom_empty(), mk_atom_empty()), list);
            }

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
        printf("unknown case: compileBool() (%d)\n", expr->type);
        break;
    }

    return;
}


void compileCmd(AST* cmd){
    if (cmd == NULL)
        return;

    switch (cmd->type) {
    case CMD:
        compileCmd(cmd->left);
        compileCmd(cmd->right);
        break;

    case ASG:
        {
            char* r = tx();
            AssignVal* expr = (AssignVal*)cmd;
            char* var = expr->sym->name;
            compileExpr( expr->val, r );

            if(!valid_register(var))
                set_register(var, s_count++);

            char* sx = get_register(var);
            add_instr( mk_instr(ATRIB, mk_atom_str(sx), mk_atom_str(r),  mk_atom_empty(), mk_atom_empty()), list);
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
            /* TODO: decide to print string, number or symbol */
            Symbol* s = ((IOFunc*)cmd)->symbol;
            char* r = get_register(s->name);
            add_instr(mk_instr(PRINT, mk_atom_str(r), mk_atom_empty(), mk_atom_empty(), mk_atom_empty()), list);
        }
        break;

    case RDL:
        {
            Symbol* s = ((IOFunc*)cmd)->symbol;
            char* r = get_register(s->name);
            add_instr(mk_instr(READ, mk_atom_str(r), mk_atom_empty(), mk_atom_empty(), mk_atom_empty()), list);
        }
        break;

    default:
        printf("unknown case: compileCmd()\n");
        break;
    }

    // Reset tmp reg counter
    // make them available to reuse
    t_count = 0;

    return;
}



int main(int argc, char** argv) {
    --argc; ++argv;

    // Initialize instruction list with "main" label
    list = mk_instr_list( mk_instr(LABEL, mk_atom_str("_main"), mk_atom_empty(), mk_atom_empty(), mk_atom_empty()), NULL);

    printf("\n====================\n");
    printf("SOURCE CODE\n");
    printf("====================\n");
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

    FILE* output = fopen("mips_program.asm", "w");

    //  yyin = stdin
    if (yyparse() == 0) {
        printf("====================\n");
        printf("ABSTRACT SYNTAX TREE\n");
        printf("====================\n");
        printAST(root);
        compileCmd(root);
    }

    // End the list with exit syscall instr
    add_instr( mk_instr(EXIT, mk_atom_empty(), mk_atom_empty(), mk_atom_empty(), mk_atom_empty()), list);

    printf("====================\n");
    printf("THREE-ADDRESS CODE (IR)\n");
    printf("====================\n");
    print_3AC(list);
    printf("====================\n");
    printf("MIPS ASSEMBLY\n");
    printf("====================\n");
    print_MIPS(list, output);
    fclose(output);

    printf("\n[+] MIPS assembly generated to file: 'mips_program.asm'\n");
    return 0;
}
