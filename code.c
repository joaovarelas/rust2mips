#include <stdio.h>
#include <stdlib.h> // for malloc
#include <string.h>
#include <stdbool.h>
#include "code.h"


Instr* mk_instr(OpKind kind, Atom* a1, Atom* a2, Atom* a3/*, Atom* a4*/){
    Instr* instr = (Instr*)malloc(sizeof(Instr));
    instr->op = kind;
    instr->a1 = a1;
    instr->a2 = a2;
    instr->a3 = a3;
    //instr->a4 = a4;
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

/*

  TO COMPLETE

*/
void print_list(InstrList* list){
    
    while(!list_is_empty(list)){

	switch(list->i->op){
	case ATRIB:
	    printf("\t(ATRIB)\n");
	    break;
	case PLUS:
	    printf("\t(PLUS)\n");
	    break;
	case MINUS:
	    printf("\t(MINUS)\n");
	    break;
	case MULTI:
	    printf("\t(MULTI)\n");
	    break;
	case LABEL:
	    printf("LABEL %s:\n", list->i->a1->u.name);
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




