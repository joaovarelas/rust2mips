#ifndef __table_h__
#define __table_h__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h> // for malloc
#include <string.h>
#include "ast.h"

// hashtable max entries (prime number to reduce possible collisions)
#define MAX_ENTRIES 10007

// Build Symbols Table (Hashtable)
Symbol hash_table[MAX_ENTRIES];
//int hash_table[MAX_ENTRIES];

unsigned int mk_hash(char *var);
Symbol* search_table(char* var);
void set_symbol_value(char* var, int value);
int get_symbol_value(char* var);


#endif
