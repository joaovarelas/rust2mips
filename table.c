#include "table.h"

// Jenkins Hash Function
unsigned int mk_hash(char* var){
    unsigned int hash = 0, c=0;
   
    while( var[c] ){
	hash += var[c];
        hash += (hash << 10);
        hash ^= (hash >> 6);
	++c;
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash -= (hash << 15);

    hash %= MAX_ENTRIES;
    // printf("Debug: hash(%s) = %u\n", var, hash);
    return hash;
}


// As of flex&bison by John Levine
Symbol* search_table(char* var){
    int scount = MAX_ENTRIES;
    Symbol* sp = &hash_table[ mk_hash(var) ];
        
    while(--scount >= 0) {
	if(sp->name && !strcmp(sp->name, var)) { return sp; }

	if(!sp->name) {		/* new entry */
	    sp->name = strdup(var);
	    sp->val = 0;
	    return sp;
	}
	
	if(++sp >= hash_table + MAX_ENTRIES)
	    sp = hash_table; /* try the next entry */
    }
    yyerror("symbol table overflow\n");
    abort(); /* tried them all, table is full */

}


// Aux
void set_symbol_value(char* var, int value){
    /*
    unsigned int hash = mk_hash(var);
    Symbol* s = &hash_table[ hash ];
    s->name = strdup(var);
    s->val = value;
    */
    Symbol* s = search_table(var);
    s->val = value;
    return;
}

int get_symbol_value(char* var){
    return (search_table(var))->val;
}

