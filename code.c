#include "code.h"
#include "ast.h"
#include "registers.h"

Instr* mk_instr(OpKind kind, Atom* a1, Atom* a2, Atom* a3, Atom* a4){
    Instr* instr = (Instr*)malloc(sizeof(Instr));
    instr->op = kind;
    instr->a1 = a1;
    instr->a2 = a2;
    instr->a3 = a3;
    instr->a4 = a4;
    return instr;
}

InstrList* mk_instr_list(Instr* i, InstrList* next){
    InstrList* instr = (InstrList*)malloc(sizeof(InstrList));
    instr->i = i;
    instr->next = NULL;
    return instr;
}

InstrList* mk_append(InstrList* l1, InstrList* l2){
    if(l1 == NULL) return l2;
    else return mk_instr_list(l1->i, mk_append(l1->next, l2));
}

void add_instr(Instr* instr, InstrList* list){
    InstrList* l = list;
    while(l->next != NULL)
        l = l->next;

    l->next = mk_instr_list(instr, NULL);
}

bool list_is_empty(InstrList* list){
    return (list == NULL) ? true : false;
}

Atom* mk_atom_empty(){
    Atom* a = (Atom*)malloc(sizeof(Atom));
    a->kind = EMPTY;
    return a;
}

Atom* mk_atom_int(int v){
    Atom* a = (Atom*)malloc(sizeof(Atom));
    a->kind = INT;
    a->u.value = v;
    return a;
}

Atom* mk_atom_str(char* v){
    Atom* a = (Atom*)malloc(sizeof(Atom));
    a->kind = STR;
    a->u.name = v;
    return a;
}


void print_3AC(InstrList* list){

    while(!list_is_empty(list)){
        Instr* i = list->i;

        Atom* a1 = i->a1;
        Atom* a2 = i->a2;
        Atom* a3 = i->a3;
        Atom* a4 = i->a4;

        switch(i->op){
        case LABEL:
            printf("%s:\n", a1->u.name);
            break;
        case ATRIB:
            printf("\t%s := ", a1->u.name);
            if (a2->kind == INT) printf("%d", a2->u.value);
            else printf("%s", a2->u.name);
            printf("\n");
            break;
        case GOTO:
            printf("\tGOTO %s\n", a1->u.name);
            break;

        case IFE:
            //printf("\tIF %s == %s GOTO %s ELSE %s\n", a1->u.name, a2->u.name, a3->u.name, a4->u.name);
            printf("\tIF %s == %s GOTO %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case IFNE:
            //printf("\tIF %s != %s GOTO %s ELSE %s\n", a1->u.name, a2->u.name, a3->u.name, a4->u.name);
            printf("\tIF %s != %s GOTO %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case IFG:
            //printf("\tIF %s > %s GOTO %s ELSE %s\n", a1->u.name, a2->u.name, a3->u.name, a4->u.name);
            printf("\tIF %s > %s GOTO %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case IFGE:
            //printf("\tIF %s >= %s GOTO %s ELSE %s\n",  a1->u.name, a2->u.name, a3->u.name, a4->u.name);
            printf("\tIF %s >= %s GOTO %s\n",  a1->u.name, a2->u.name, a3->u.name);
            break;
        case IFL:
            //printf("\tIF %s < %s GOTO %s ELSE %s\n", a1->u.name, a2->u.name, a3->u.name, a4->u.name);
            printf("\tIF %s < %s GOTO %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case IFLE:
            //printf("\tIF %s <= %s GOTO %s ELSE %s\n", a1->u.name, a2->u.name, a3->u.name, a4->u.name);
            printf("\tIF %s <= %s GOTO %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;

        case PLUS:
            printf("\t%s := %s + %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case MINUS:
            printf("\t%s := %s - %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case MULTI:
            printf("\t%s := %s * %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case DIVI:
            printf("\t%s := %s / %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case MODUL:
            printf("\t%s := %s %% %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;

        case PRINTS:
            printf("\tPRINTS %s\n", a1->u.name);
            break;
        case PRINT:
            printf("\tPRINT %s\n", a1->u.name);
            break;
        case READ:
            printf("\tREAD %s\n", a1->u.name);
            break;
        case EXIT:
            printf("\tEXIT\n");
            break;

        default:
            printf("unknown case: print_3AC()\n");
            break;
        }

        list = list->next;
    }
}





void print_MIPS(InstrList* list, FILE* output){

    printf("\t.text\n");
    fprintf(output, "\t.text\n");

    while(!list_is_empty(list)){
        Instr* i = list->i;

        Atom* a1 = i->a1;
        Atom* a2 = i->a2;
        Atom* a3 = i->a3;
        Atom* a4 = i->a4;

        switch(i->op){
        case LABEL:
            printf("%s:\n", a1->u.name);
            fprintf(output, "%s:\n", a1->u.name);
            break;

        case ATRIB:
            if(a2->kind == INT) {
              printf("\tli $%s, %d\n", a1->u.name, a2->u.value);
              fprintf(output, "\tli $%s, %d\n", a1->u.name, a2->u.value);
            }
            else {
              printf("\tmove $%s, $%s\n", a1->u.name, a2->u.name);
              fprintf(output, "\tmove $%s, $%s\n", a1->u.name, a2->u.name);
            }
            break;

        case GOTO:
            printf("\tj %s\n", a1->u.name);
            fprintf(output, "\tj %s\n", a1->u.name);
            break;

        case IFE:
            printf("\tbeq $%s, $%s, %s\n", a1->u.name, a2->u.name, a3->u.name);
            fprintf(output, "\tbeq $%s, $%s, %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case IFNE:
            printf("\tbne $%s, $%s, %s\n", a1->u.name, a2->u.name, a3->u.name);
            fprintf(output, "\tbne $%s, $%s, %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case IFG:
            printf("\tbgt $%s, $%s, %s\n", a1->u.name, a2->u.name, a3->u.name);
            fprintf(output, "\tbgt $%s, $%s, %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case IFGE:
            printf("\tbge $%s, $%s, %s\n", a1->u.name, a2->u.name, a3->u.name);
            fprintf(output, "\tbge $%s, $%s, %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case IFL:
            printf("\tblt $%s, $%s, %s\n", a1->u.name, a2->u.name, a3->u.name);
            fprintf(output, "\tblt $%s, $%s, %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case IFLE:
            printf("\tble $%s, $%s, %s\n", a1->u.name, a2->u.name, a3->u.name);
            fprintf(output, "\tble $%s, $%s, %s\n", a1->u.name, a2->u.name, a3->u.name);
            break;

        case PLUS:
            printf("\tadd $%s, $%s, $%s\n", a1->u.name, a2->u.name, a3->u.name);
            fprintf(output, "\tadd $%s, $%s, $%s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case MINUS:
            printf("\tsub $%s, $%s, $%s\n", a1->u.name, a2->u.name, a3->u.name);
            fprintf(output, "\tsub $%s, $%s, $%s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case MULTI:
            printf("\tmul $%s, $%s, $%s\n", a1->u.name, a2->u.name, a3->u.name);
            fprintf(output, "\tmul $%s, $%s, $%s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case DIVI:
            printf("\tdiv $%s, $%s, $%s\n", a1->u.name, a2->u.name, a3->u.name);
            fprintf(output, "\tdiv $%s, $%s, $%s\n", a1->u.name, a2->u.name, a3->u.name);
            break;
        case MODUL:
            printf("\trem $%s, $%s, $%s\n", a1->u.name, a2->u.name, a3->u.name);
            fprintf(output, "\trem $%s, $%s, $%s\n", a1->u.name, a2->u.name, a3->u.name);
            break;

        case PRINTS:
            /* need .data segment */
            printf("\tPRINTS FIX\n");
            fprintf(output, "\tnop\n");
            break;

        case PRINT:
            // Print Integer Syscall
            // TODO: This should print strings? We don't have them
            // TODO: Check atom kind and handle non-variables
            printf("\tli $v0, 1\n");
            printf("\tadd $a0, $%s, $zero\n", a1->u.name);
            printf("\tsyscall\n");
            fprintf(output, "\tli $v0, 1\n");
            fprintf(output, "\tadd $a0, $%s, $zero\n", a1->u.name);
            fprintf(output, "\tsyscall\n");
            break;

        case READ:
            // Read Integer Syscall
            printf("\tli $v0, 5\n");
            printf("\tsyscall\n");
            printf("\tadd $%s, $v0, $zero\n", a1->u.name);
            fprintf(output, "\tli $v0, 5\n");
            fprintf(output, "\tsyscall\n");
            fprintf(output, "\tadd $%s, $v0, $zero\n", a1->u.name);
            break;
        case EXIT:
            printf("\tli $v0, 10\n");
            printf("\tsyscall\n");
            fprintf(output, "\tli $v0, 10\n");
            fprintf(output, "\tsyscall\n");
            break;
        default:
            // Unknown case? NOP! AAHAH XD
            printf("\tnop\n");
            fprintf(output, "\tnop\n");
            break;
        }

        list = list->next;
    }
}
