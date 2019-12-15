#include "registers.h"

int ndigits(int x){
    return (x == 0) ? 1 : floor(log(x))+1 ;
}

int s_count = 0;
char* sx(){
    if(s_count > 7)
        printf("Error: s_count exceeded\n");
    char* s = (char*)malloc(sizeof( ndigits(s_count) * sizeof(char) ));
    sprintf(s, "s%d", s_count++);
    return s;
}


int t_count = 0;
char* tx(){
    if(t_count > 9)
        printf("Error: t_count exceeded\n");
    char* t = (char*)malloc(sizeof( ndigits(t_count) * sizeof(char) ));
    sprintf(t, "t%d", t_count++);
    return t;
}


int l_count = 0;
char* lx(){
    char* l = (char*)malloc(sizeof( ndigits(l_count) * sizeof(char) ));
    sprintf(l, "_L%d", l_count++);
    return l;
}
