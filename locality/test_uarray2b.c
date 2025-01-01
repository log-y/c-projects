#include "uarray2b.h"
#include <stdlib.h>
#include <stdio.h>

void apply(int col, int row, UArray2b_T array2b, void* elem, void *cl){
        char* c = elem;
        char* c1 = UArray2b_at(array2b, col, row);
        (void)c1;
        (void)cl;
        printf("[%d,%d]%c \n", row, col, *c);
}

int main() 
{
        /* regular numbers */
        UArray2b_T test_1 = UArray2b_new(5, 5, sizeof(char), 3);
        /* when had 12,30, could successfully make 360 elements */
        /* bad numbers */
        // UArray2b_T test_2 = UArray2b_new(0, 1, sizeof(char), 1);
        for (int row = 0; row < 5; row++) {
                for (int col = 0; col < 5; col++) {
                        char* c = UArray2b_at(test_1, col, row);
                        *c = 'A' + (col + row); // Assign a letter based on col and row
                }
        }
        char* c = UArray2b_at(test_1, 4, 4);
        *c = 'K';
        UArray2b_map(test_1, apply, NULL);

        UArray2b_free(&test_1);
        // UArray2b_free(&test_2);
        // (void)test_1;

        

        exit(EXIT_SUCCESS);
}
