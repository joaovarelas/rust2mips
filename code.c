#include <stdio.h>
#include <stdlib.h> // for malloc
#include <string.h>
#include <stdbool.h>
#include "code.h"
#include "ast.h"

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


void print_list(InstrList* list){
    
    while(!list_is_empty(list)){
        Instr* i = list->i;
        
	switch(i->op){
        case LABEL:
	    printf("LABEL %s:\n", list->i->a1->u.name);
	    break;
	case ATRIB:
            printf("\t%s := ", i->a1->u.name);
            if(i->a2->kind == INT) printf("%d", i->a2->u.value);
            else printf("%s", i->a2->u.name);
            printf("\n");
            break;
        case GOTO:
            printf("\tGOTO %s\n", i->a1->u.name);
            break;
        case IFE:
            printf("\tIF %s == %s THEN GOTO %s\n",
                   i->a1->u.name, i->a2->u.name, i->a3->u.name);
            break;
        case IFDIF:
	    printf("\tIFDIF\n");
	    break;
        case IFG:
	    printf("\tIFG\n");
	    break;
        case IFGE:
	    printf("\tIFGE\n");
	    break;
        case IFL:
	    printf("\tIFL\n");
	    break;
        case IFLE:
	    printf("\tIFLE\n");
	    break;
        case PLUS:
            printf("\t%s := %s + %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
            break;
	case MINUS:
	    printf("\t%s := %s - %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
	    break;
	case MULTI:
	    printf("\t%s := %s * %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
	    break;
        case DIVI:
	    printf("\t%s := %s / %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
	    break;
        default:
	    printf("unknown case: print_list()\n");
	    break;
	}
	list = list->next;
    }
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




