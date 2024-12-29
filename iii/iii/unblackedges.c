#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include <stack.h>
#include <string.h>
#include <pnmrdr.h>

#include "bit2.h"

#define T Bit2_T

T read_pbm(FILE *file);
void flood_fill(T map, int start_row, int start_col, int rows, int cols);
void pbmwrite(FILE *outputfp, T bitmap);


int main(int argc, char *argv[])
{
        /* makes sure right amount of files */
        assert(argc <= 2 && argc > 0);
        T map = NULL;
        if(argc > 1){
                FILE *file = fopen(argv[1], "rb");
                assert(file != NULL);
                map = read_pbm(file);
        }
        else{
                map = read_pbm(stdin);
        }
        // FILE *file = argc > 1 ? fopen(argv[1], "rb") : stdin;
        // assert(file != NULL);

        // T map = read_pbm(file);

        int h = Bit2_height(map);
        int w = Bit2_width(map);

        // printf("\n");
        for(int i = 0; i < Bit2_height(map); i++){
                for(int j = 0; j < Bit2_width(map); j++){
                        int ele = Bit2_get(map, j, i);
                        printf("%d ", ele);
                }
                printf("\n");
        }

        // return 1;
        
        for(int i = 0; i < Bit2_height(map); i++){
                flood_fill(map, i, 0, h, w);
                flood_fill(map, i, w - 1, h, w);
        }

        for(int i = 0; i < Bit2_width(map); i++){
                flood_fill(map, 0, i, h, w);
                flood_fill(map, h - 1, i, h, w);
        }

        printf("\n");
        for(int i = 0; i < Bit2_height(map); i++){
                for(int j = 0; j < Bit2_width(map); j++){
                        int ele = Bit2_get(map, j, i);
                        printf("%d ", ele);
                }
                printf("\n");
        }
        pbmwrite(stdout, map);
}

void skip_comments(FILE *file) {
    char line[1024];  // Buffer for reading lines

    // Loop until a non-comment, non-whitespace line is found
    while (fgets(line, sizeof(line), file)) {
        // If the line starts with '#' (comment) or is blank, skip it
        if (line[0] == '#') {
            continue;
        }

        // Check if the line contains non-whitespace characters
        int i = 0;
        while (isspace(line[i])) i++;  // Skip leading whitespace
        if (line[i] != '\0') {
            // If the line is not empty or whitespace, move the file pointer back
            fseek(file, -(long)strlen(line), SEEK_CUR);  // Move back to the start of this line
            break;
        }
    }
}

/********** read_pgm ********
 *
 *      Reads from an input (file or stdin) and stores the input data inside
 *      of a UArray2. Function also checks formatting of file
 *
 * Parameters:
 *      FILE *file: is a file readerof the opened file
 * Return: 
 *      UArray_T: represents the sudoku board stored in a UArray_T
 * Expects
 *      *file is not NULL
 * Notes:
 *      Will CRE if there is any invalid formatting (does not constrain to
 *      the pgm formatting), will CRE if there is invalid number read in (not
 *      a sudoku number)
 ************************/
T read_pbm(FILE *file)
{
        assert(file != NULL);
        assert(!ferror(file));


        char magic_number[3];
        int width, height;
        
        /* Read and check magic number */
        fscanf(file, "%2s", magic_number);
        assert(magic_number[0] == 'P' && magic_number[1] == '1');

        skip_comments(file);

        /* Read and check proportions */
        fscanf(file, "%d %d", &width, &height);
        assert(width > 0 && height > 0);
        
        T map = Bit2_new(width, height);

        for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                       int value;
                       int readIn = fscanf(file, "%d", &value);
                       assert(readIn == 1 && (value == 0 || value == 1));
                       int prev = Bit2_put(map, j, i, value);
                       (void)prev;
                }
        }

        int ch; /* account for any trailing whitespace */
        while ((ch = fgetc(file)) != EOF) {
                assert(isspace(ch));
        }
        
        fclose(file);

        return map;
}

// Helper functions to encode and decode coordinates
/********** encode_coordinates ********
 *
 *      x
 *
 * Parameters:
 *      x
 * Return: 
 *      x
 * Expects
 *      x
 * Notes:
 *      x
 ************************/
int encode_coordinates(int row, int col, int cols) {
        return row * cols + col;
}

/********** decode_coordinates ********
 *
 *      x
 *
 * Parameters:
 *      x
 * Return: 
 *      x
 * Expects
 *      x
 * Notes:
 *      x
 ************************/
void decode_coordinates(int encoded, int cols, int *row, int *col) {
        *row = encoded / cols;
        *col = encoded % cols;
}

/********** flood_fill ********
 *
 *      x
 *
 * Parameters:
 *      x
 * Return: 
 *      x
 * Expects
 *      x
 * Notes:
 *      x
 ************************/
// Flood-fill algorithm using the provided stack interface
void flood_fill(T map, int start_row, int start_col, int rows, int cols) {
        if(start_row > rows || start_col > cols){
                return;
        }
        Stack_T stack = Stack_new();  // Create a new stack

        // Encode the starting pixel coordinates and push it onto the stack
        int start_encoded = encode_coordinates(start_row, start_col, cols);
        Stack_push(stack, (void *)(intptr_t)start_encoded);  // Cast encoded int to void*

        // Process the stack while it's not empty
        while (!Stack_empty(stack)) {
                // Pop the current encoded pixel from the stack
                int encoded = (intptr_t)Stack_pop(stack);  // Cast void* back to int
                int r, c;
                decode_coordinates(encoded, cols, &r, &c);
                // printf("attempting to get c=%d r=%d \n", c, r);
                // Check if the pixel is black (0)
                if (Bit2_get(map, c, r) == 1) {
                        // printf("black pixel detected at row %d, col %d\n", r, c);
                        // Turn the current pixel white (set it to 1)
                        Bit2_put(map, c, r, 0);

                        // Push neighboring pixels onto the stack if they are in bounds
                        if (r > 0) {  // up
                                // printf("try going up \n");
                                int up_encoded = encode_coordinates(r - 1, c, cols);
                                Stack_push(stack, (void *)(intptr_t)up_encoded);
                        }
                        if (r < rows - 1) {  // down
                                // printf("try going down \n");
                                int down_encoded = encode_coordinates(r + 1, c, cols);
                                Stack_push(stack, (void *)(intptr_t)down_encoded);
                        }
                        if (c > 0) {  // left
                                // printf("try going left \n");
                                int left_encoded = encode_coordinates(r, c - 1, cols);
                                Stack_push(stack, (void *)(intptr_t)left_encoded);
                                // printf("pushed sduccessfully \n");
                        }
                        if (c < cols - 1) {  // right
                                // printf("try going right \n");
                                int right_encoded = encode_coordinates(r, c + 1, cols);
                                Stack_push(stack, (void *)(intptr_t)right_encoded);
                        }
                }
        }
        // Free the stack after processing
        Stack_free(&stack);
}

/********** flood_fill ********
 *
 *      x
 *
 * Parameters:
 *      x
 * Return: 
 *      x
 * Expects
 *      x
 * Notes:
 *      x
 ************************/
void write_bit(int row, int col, T bitArray, int curr, void *cl) {
        FILE *outputfp = (FILE *)cl;
        fprintf(outputfp, "%d", curr);  // Write each bit (either 0 or 1)
        
        // row = row;
        (void)row;
        if (col == Bit2_width(bitArray) - 1) {
                // End of row, print newline
                fprintf(outputfp, "\n");
        }
}

/********** flood_fill ********
 *
 *      x
 *
 * Parameters:
 *      x
 * Return: 
 *      x
 * Expects
 *      x
 * Notes:
 *      x
 ************************/
void pbmwrite(FILE *outputfp, T bitmap) {
        assert(outputfp != NULL);
        assert(bitmap != NULL);

        // Write the PBM header
        fprintf(outputfp, "P1\n");  // Magic number for ASCII PBM (P1 format)
        fprintf(outputfp, "%d %d\n", Bit2_width(bitmap), Bit2_height(bitmap));  // Dimensions
    
        // Write bitmap contents using row-major order
        Bit2_map_row_major(bitmap, write_bit, outputfp);
}


/*
        // Pnmrdr_T reader = Pnmrdr_new(file);
        // Pnmrdr_mapdata data = Pnmrdr_data(reader);
        // assert(data.type == Pnmrdr_bit);

        // int width = data.width;
        // int height = data.height;

        // T map = Bit2_new(width, height);

        // assert(width > 0 && height > 0);

        // // int total_size = width * height;
        // for(int i = 0; i < height; i++){
        //         for(int j = 0; j < width; j++){
        //                 int d = Pnmrdr_get(reader);
        //                 assert(d == 0 || d == 1);
        //                 // printf("%d", d);
        //                 int prev = Bit2_put(map, j, i, (int)d);
        //                 (void) prev;
        //         }
        // }
        // Pnmrdr_free(&reader);

*/