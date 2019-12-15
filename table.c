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
Symbol* search_table(Symbol* table, char* var){
    int scount = MAX_ENTRIES;
    Symbol* sp = &table[ mk_hash(var) ];
        
    while(--scount >= 0) {
	if(sp->name && !strcmp(sp->name, var)) { return sp; }

	if(!sp->name) {		/* new entry */
	    sp->name = strdup(var);
	    sp->val = 0;
	    return sp;
	}
	
	if(++sp >= table + MAX_ENTRIES)
	    sp = table; /* try the next entry */
    }
    yyerror("symbol table overflow\n");
    abort(); /* tried them all, table is full */

}



// Aux
int get_symbol(char* var){
    return (search_table(sym_table, var))->val;
}

void set_symbol(char* var, int value){
    Symbol* s = search_table(sym_table, var);
    s->val = value;
    return;
}



char* get_register(char* var){
    char r[3] = {0,0,0};
    Symbol* s = search_table(reg_table, var);
    sprintf(r, "s%d", s->val);
    return strdup(r);
}

void set_register(char* var, int sx){
    Symbol* s = search_table(reg_table, var);
    s->val = sx;
    return;
}
