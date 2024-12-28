/*
Test.c
The purpose of this was to use the Pnmrdr "class" to see if we could
open and close a p5 file. This would help us know if we printed the
format correctly.
Takes in arguments from console and is made separately using 'make test'.

*/

#include <stdlib.h>
#include <pnmrdr.h>

/*
name: main
purpose: run the actual testing. creates a pnmrdr object, checks if it
opened correctly, then prints each pixel.
this throws errors if we did not implement something correctly, like incorrect
format, or 'too few or too many' pixels.
*/
int main(int argc, char *argv[]){
        if(argc != 2){
                return 0;
        }
        FILE *img;
        img = fopen(argv[1], "rb");
        Pnmrdr_T reader = Pnmrdr_new(img); /* checks creation, can throw error*/
        Pnmrdr_mapdata data = Pnmrdr_data(reader);
        if (data.type != Pnmrdr_gray) { /* checks format */
                fprintf(stderr, "Wrong file format.\n");
                Pnmrdr_free(&reader);
                fclose(img);
                exit(EXIT_FAILURE);
        }
        printf("Data.width is %u ", data.width); /* print dimensions */
        printf("Data.height is %u ", data.height);
        int num_pix = data.width * data.height;
        printf("P5 ");
        printf("%d ", data.width);
        printf("%d ", data.height);
        for (int i = 0; i < num_pix; i++) { /* print each pixel and position*/
                printf("printing %d out of %d. dimensions %d x %d \n", i, num_pix, data.width, data.height);
                printf("%d ", Pnmrdr_get(reader));
        }
        return 0;
}