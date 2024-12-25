#include "tul.h"
#include <stdio.h>
#include <stdlib.h>



/*
 * Uses sscanf() to read three unsigned long integers from
 * inputline, and places the results in the fields of the
 * structure pointed to by p. If sscanf() fails to read these 
 * three values, explode the bomb!   
 */
void read_three_unsigned_longs(const char *inputline,
                               struct three_unsigned_longs *p){
       int read = sscanf(inputline, "%lu %lu %lu", &(p->a), &(p->b), &(p->c));
       if(read != 3){
                explode_bomb();
       }
}


void explode_bomb(void)
{
        printf("Boom (just for practice)!\n");
        exit(1);
}