#ifndef __table_h__
#define __table_h__

#include "ast.h"

// hashtable max entries (prime number to reduce possible collisions)
#define MAX_ENTRIES 10007

// Build Symbols Table (Hashtable)
Symbol sym_table[MAX_ENTRIES];
Symbol reg_table[MAX_ENTRIES];

unsigned int mk_hash(char *var);
Symbol* search_table(Symbol* table, char* var);

void set_symbol(char* var, int value);
int get_symbol(char* var);

void set_register(char* var, int sx);
char* get_register(char* var);
bool valid_register(char* var);

#endif
