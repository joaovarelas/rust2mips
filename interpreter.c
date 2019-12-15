#include <stdio.h>
#include <stdbool.h>
#include "parser.h"
#include "interpreter.h"

extern void printAST(AST* root);

void indent(int depth){
  for(int n = 0; n++ < depth; printf("    "));
  return;
}

int eval(AST* ast, int depth, bool flag){

  if(ast == NULL){
    printf("NULL AST\n");
    return -1;}


    int result;
    char* name;
    AST* val;
    AST* cond;
    AST* then_block;
    AST* else_block;

    switch (ast->type) {

      // List of CMDs
      case CMD:
      //indent(depth);    printf("<command>\n");
      if(ast->left != NULL) eval(ast->left, depth, flag);
      if(ast->right != NULL) eval(ast->right, depth, flag);
      break;



      // Terminal Symbols
      case NUM:
      result = ((IntVal*)ast)->num;
      if(flag){ indent(depth);    printf("<number %d>\n", result);}
      break;

      case SYM:
      result = ((SymbolRef*)ast)->sym->val;
      name = ((SymbolRef*)ast)->sym->name;
      if(flag){ indent(depth);    printf("<symbol %s = %d>\n", name, result);}
      break;



      // Variable Assignment
      case ASG:
      result = 0;
      name = ((AssignVal*)ast)->sym->name;
      val = ((AssignVal*)ast)->val;
      if(flag){ indent(depth);      printf("<assignment>\n");}
      if(flag){ indent(depth+1);    printf("<variable %s>\n", name);}
      ((AssignVal*)ast)->sym->val = eval( val , depth+1, flag);
      break;



      // IO Func
      case PTL:
      result = 0;
      if(flag){ indent(depth);        printf("<println!>\n");}
      if ( ((IOFunc*)ast)->symbol ) {
        result = ((SymbolRef*)ast)->sym->val;
        name = ((SymbolRef*)ast)->sym->name;
        if(flag){ indent(depth+1);    printf("<symbol %s = %d>\n", name, result);}
      }
      else if ( ((IOFunc*)ast)->number ) {
        result = ((IntVal*)ast)->num;
        if(flag){ indent(depth+1);    printf("<number %d>\n", result);}
      }
      else if ( ((IOFunc*)ast)->string ) {
        name = ((IOFunc*)ast)->string;
        if(flag){ indent(depth+1);    printf("<string %s>\n", name);}
      }
      break;


      case RDL:
      result = 0;
      if(flag){ indent(depth);        printf("<read_line>\n");}
      int read_val;
      name = ((SymbolRef*)ast)->sym->name;
      if ( ((IOFunc*)ast)->symbol ) {
        char buf[64];
        fgets(buf, 64, stdin);
        read_val = atoi(buf);
        ((SymbolRef*)ast)->sym->val = read_val;
      }
      if(flag){  indent(depth+1);    printf("<symbol %s = %d>\n", name, read_val);}
      break;




      // Control Flow
      case IFS:
      result = 0;
      cond = ((ControlFlow*)ast)->cond;
      then_block = ((ControlFlow*)ast)->then_block;
      else_block = ((ControlFlow*)ast)->else_block;

      // Print
      if(flag){
        indent(depth);        printf("<if_statement>\n");
        indent(depth+1);      printf("<cond>\n");
        eval(cond, depth+2, true);
        if(then_block){
          indent(depth+1);    printf("<then>\n");
          eval(then_block , depth+2, flag);
        }

        if(else_block){
          indent(depth+1);    printf("<else>\n");
          eval(else_block , depth+2, flag);
        }
      }

      // Execute
      if(eval(cond, depth+2, false) != 0 ){
        if(then_block){
          eval(then_block , depth+2, false);
        }
      } else{
        if(else_block){
          eval(else_block , depth+2, false);
        }
      }

      break;


      case WHS:
      result = 0;
      cond = ((ControlFlow*)ast)->cond;
      then_block = ((ControlFlow*)ast)->then_block;

      // Print
      if(flag){
        indent(depth);        printf("<while_statement>\n");
        indent(depth+1);      printf("<cond>\n");
        eval( cond , depth+2, true);

        if( then_block ){
          indent(depth+1);    printf("<then>\n");
          eval( then_block , depth+2, true);
        }
      }

      // Execute
      while ( eval(cond , depth+2, false) != 0 ){
        if( then_block ){
          eval( then_block , depth+2, false);
        }
      }
      break;



      // Arithmetic Ops
      case ADD:
      if(flag){ indent(depth);    printf("<add>\n");}
      result = eval(ast->left, depth+1, flag) + eval(ast->right, depth+1, flag);
      break;

      case SUB:
      if(flag){ indent(depth);    printf("<sub>\n");}
      result = eval(ast->left, depth+1, flag) - eval(ast->right, depth+1, flag);
      break;

      case MULT:
      if(flag){ indent(depth);    printf("<mult>\n");}
      result = eval(ast->left, depth+1, flag) * eval(ast->right, depth+1, flag);
      break;

      case DIV:
      if(flag){ indent(depth);    printf("<div>\n");}
      result = eval(ast->left, depth+1, flag) / eval(ast->right, depth+1, flag);
      break;

      case MOD:
      if(flag){ indent(depth);    printf("<mod>\n");}
      result = eval(ast->left, depth+1, flag) % eval(ast->right, depth+1, flag);
      break;



      // Logic operators
      case AND:
      if(flag){ indent(depth);    printf("<and>\n");}
      result = eval(ast->left, depth+1, flag) && eval(ast->right, depth+1, flag);
      break;

      case OR:
      if(flag){ indent(depth);    printf("<or>\n");}
      result = eval(ast->left, depth+1, flag) || eval(ast->right, depth+1, flag);
      break;

      case EQT:
      if(flag){ indent(depth);    printf("<equal>\n");}
      result = eval(ast->left, depth+1, flag) == eval(ast->right, depth+1, flag);
      break;

      case NEQ:
      if(flag){ indent(depth);    printf("<notequal>\n");}
      result = eval(ast->left, depth+1, flag) != eval(ast->right, depth+1, flag);
      break;

      case GRT:
      if(flag){ indent(depth);    printf("<greater>\n");}
      result = eval(ast->left, depth+1, flag) > eval(ast->right, depth+1, flag);
      break;

      case LRT:
      if(flag){ indent(depth);    printf("<less>\n");}
      result = eval(ast->left, depth+1, flag) < eval(ast->right, depth+1, flag);
      break;

      case GEQ:
      if(flag){ indent(depth);    printf("<greatorequal>\n");}
      result = eval(ast->left, depth+1, flag) >= eval(ast->right, depth+1, flag);
      break;

      case LEQ:
      if(flag){ indent(depth);    printf("<lessorequal>\n");}
      result = eval(ast->left, depth+1, flag) <= eval(ast->right, depth+1, flag);
      break;

      default:
      yyerror("Switch error");
    }

    return result;
  }


void printAST(AST* root){
    printf("<main>\n");
    int _ = eval(root, 1, true);
    return;
}

/*
  int main(int argc, char** argv) {
    --argc; ++argv;
    if (argc != 0) {
      yyin = fopen(*argv, "r");
      if (!yyin) {
        printf("'%s': could not open file\n", *argv);
        return 1;
      }
    }

    //  yyin = stdin
    if (yyparse() == 0) {
      printf("<main>\n");
      int _ = eval(root, 1, true);
    }
    return 0;


  }
*/
