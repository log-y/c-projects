#include <stdlib.h>
#include <stdio.h>
#include "pnmrdr.h"


extern void readFile(FILE *fp);

int main(int argc, char *argv[])
{
    if (argc == 1) {
            readFile(stdin);
    } else {
            for (int i = 1; i < argc; i++) {
                    FILE *fp = fopen(argv[i], "r");
                    if (fp == NULL) {
                            fprintf(stderr, 
                                    "%s: %s %s %s\n",
                                    argv[0], "Could not open file",
                                    argv[i], "for reading");
                            exit(1);
                    }
                    readFile(fp);
                    fclose(fp);
            }
    }
    return EXIT_SUCCESS;
}

void readFile(FILE *fp){
    Pnmrdr_T f = Pnmrdr_new(fp);
    unsigned m = Pnmrdr_get(f);
    
    printf("%u", m);
}