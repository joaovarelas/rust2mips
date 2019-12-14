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

    switch(i->op){
      case LABEL:
      printf("%s:\n", list->i->a1->u.name);
      break;
      case ATRIB:
      printf("\t%s := ", i->a1->u.name);
      if (i->a2->kind == INT) printf("%d", i->a2->u.value);
      else printf("%s", i->a2->u.name);
      printf("\n");
      break;
      case GOTO:
      printf("\tGOTO %s\n", i->a1->u.name);
      break;

      case IFE:
      printf("\tIF %s == %s GOTO %s ELSE %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name, i->a4->u.name);
      break;
      case IFNE:
      printf("\tIF %s != %s GOTO %s ELSE %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name, i->a4->u.name);
      break;
      case IFG:
      printf("\tIF %s > %s GOTO %s ELSE %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name, i->a4->u.name);
      break;
      case IFGE:
      printf("\tIF %s >= %s GOTO %s ELSE %s\n",  i->a1->u.name, i->a2->u.name, i->a3->u.name, i->a4->u.name);
      break;
      case IFL:
      printf("\tIF %s < %s GOTO %s ELSE %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name, i->a4->u.name);
      break;
      case IFLE:
      printf("\tIF %s <= %s GOTO %s ELSE %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name, i->a4->u.name);
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
      printf("unknown case: print_3AC()\n");
      break;
    }
    list = list->next;
  }
}

void print_MIPS(InstrList* list){

  while(!list_is_empty(list)){
    Instr* i = list->i;

    switch(i->op){
      case LABEL:
      {
        printf("%s:\n", list->i->a1->u.name);
        break;
      }
      case ATRIB:
      {
        if (i->a2->kind == INT) {
          printf("li %s, %d\n", i->a1->u.name, i->a2->u.value);
        } else {
          printf("lw %s, 8(%s)\n", i->a1->u.name, i->a2->u.name);
        }
      }
      break;
      case GOTO:
      {
        printf("j %s\n", i->a1->u.name);
        break;
      }
      case IFE:
      {
        printf("beq %s, %s, %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
        break;
      }
      case IFNE:
      {
        printf("bne %s, %s, %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
        break;
      }
      case IFG:
      {
        printf("bgt %s, %s, %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
        break;
      }
      case IFGE:
      {
        printf("bge %s, %s, %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
        break;
      }
      case IFL:
      {
        printf("blt %s, %s, %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
        break;
      }
      case IFLE:
      {
        printf("bge %s, %s, %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
        break;
      }
      case PLUS:
      {
        printf("add %s, %s, %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
        break;
      }
      case MINUS:
      {
        printf("sub %s, %s, %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
        break;
      }
      case MULTI:
      {
        printf("mult %s, %s, %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
        break;
      }
      case DIVI:
      {
        printf("div %s, %s, %s\n", i->a1->u.name, i->a2->u.name, i->a3->u.name);
        break;
      }
      case PRINT:
      {
        // Print Integer Syscall
        // TODO: This should print strings? We don't have them
        // TODO: Check atom kind and handle non-variables
        printf("li $v0, 1\n");
        printf("add $a0, %s, $zero\n", i->a1->u.name);
        printf("syscall\n");
        break;
      }
      case READ:
      {
        // Read Integer Syscall
        printf("li $v0, 5\n");
        printf("syscall\n");
        printf("add %s, $v0, $zero\n", i->a1->u.name);
        break;
      }
      default:
      printf("unknown case: print_MIPS()\n");
      break;
    }
    list = list->next;
  }
}
