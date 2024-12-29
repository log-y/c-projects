/**************************************************************
 *
 *                     sudoku.c
 *
 *     Assignment: iii
 *     Authors:  lyuan04, dperki01, 
 *     Date:     9/25/24
 *
 *      Implements a sudoku solution checker using UArray2. Reads in a 
 *      file in a certain pgm format, then checks if the pgm contains a 
 *      9x9 grid of numbers that constitutes a valid sudoku solution
 * 
 *      Our central data structure is our UArray2, which we use to store 
 *      all of our data contiguously. We fill the UArray2 with the pgm 
 *      data, then check if the UArray2 is correct.
 *     
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>


#include "uarray2.h"

#define T UArray2_T
#define SIZE 81
#define DIMENSIONS 9

T read_pgm(FILE *file);
int is_solved(T array);
int is_valid(int *answerSet);
void get_subgrid(T array, int subgrid[DIMENSIONS], int column, int row);
int valid_subgrid(T array);

/********** main ********
 *
 *      Calls the other functions that will read in a pgm board and check if
 *      it's a valid sudoku solution
 *
 * Parameters:
 *      int argc: number of arguments from command line
 *      char* argv[]: commands given, either a file name or nothing
 * Return: 
 *      int (1 or 0) depending on whether sudoku is valid or not
 * Expects
 *      No more than 1 argument should be given
 * Notes:
 *      Will CRE for many reasons, most notably: invalid file formatting, 
 *      incorrect numbers in file.
 ************************/
int main(int argc, char *argv[])
{
        assert(argc <= 2 && argc > 0);

        FILE *file = argc > 1 ? fopen(argv[1], "r") : stdin;
        assert(file != NULL);
        T sudoku = read_pgm(file);

        int ch; /* account for any trailing whitespace */
        while ((ch = fgetc(file)) != EOF) {
                assert(isspace(ch));
        }
        fclose(file);

        int solved = is_solved(sudoku);

        UArray2_free(&sudoku);

        return solved;
}

/********** is_solved ********
 *
 *      Iterates through the columns and row of a sudoku table (stored in 
 *      a UArray2_T (which is typedefed to T so it's easier for us to 
 *      type)). Returns a 1 or 0 based on whether it was solved correctly.
 * 
 *      How it works: An array will represent each group of 9 numbers we 
 *      want to check. We can store each number in each group in the array.
 *      We then check that array for any duplicates. We do this for every
 *      group we need to check.
 *
 * Parameters:
 *      UArray2_T array: Pointer to a UArray2 that holds our sudoku table
 * Return: 
 *      int: represents whether the program was successful or not
 * Expects
 *      *array is not NULL
 * Notes:
 *      Will CRE if array is NULL, helper functions will also CRE (e.g., if
 *      there is an invalid number/character)
 ************************/
int is_solved(T array)
{
        assert(array != NULL);
        int row[DIMENSIONS], column[DIMENSIONS];
        
        for (int i = 0; i < DIMENSIONS; i++) {
                for (int j = 0; j < DIMENSIONS; j++) {
                        row[j] = *(int *)UArray2_at(array, j, i);  // Row i
                        column[j] = *(int *)UArray2_at(array, i, j);  // Column i
                }
                if (!is_valid(row) || !is_valid(column)) {
                        return 1; //maybe make this EXIT_FAILURE ? 
                }
        }

        if (!valid_subgrid(array)) {
                return 1;
        }
        return 0; //make this EXIT_SUCCESS
}

/********** get_subgrid ********
 *
 *      Helper function for valid_subgrid. Given an index of the top-left 
 *      square in any 3x3 sub-grid, this stores all of the numbers of that 
 *      sub-grid in an array.
 *
 * Parameters:
 *      UArray2_T array: represents our sudoku board
 *      int subgrid[DIMENSIONS]: represents an array we fill with numbers in 3x3
 *      int column: represents column of top-left index
 *      int row: represents row of top-left index
 * Return: 
 *      Nothing
 * Expects
 *      *array is not NULL, column and row are valid functions, subgrid isn't
 *      NULL
 * Notes:
 *      None
 ************************/
void get_subgrid(T array, int subgrid[DIMENSIONS], int column, int row)
{
        int index = 0;  
        for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                        subgrid[index++] = *(int *)UArray2_at(array, 
                                                              column + j, 
                                                              row + i);
                }
        }
}

/********** is_valid ********
 *
 *      Checks if an integer array has any duplicates. If it has no duplicates,
 *      it is considered 'valid' and returns 1, otherwise 0.
 *
 * Parameters:
 *      int* answerSet: An array of ints representing a group of numbers
 *      we have to check (like a row, col, or 3x3 subgrid)
 * Return: 
 *      int representing its validity (1 is valid, 0 is not)
 * Expects
 *      *answerSet is not NULL
 * Notes:
 *      NULL
 ************************/
int is_valid(int *answerSet) {
        int count[DIMENSIONS + 1] = {0};
        for (int i = 0; i < DIMENSIONS; i++) {
                count[answerSet[i]]++;
                if (answerSet[i] < 1 || answerSet[i] > 9 || count[answerSet[i]] > 1) {
                        return 0; 
                }
        }
        return 1; 
}

/********** valid_subgrid ********
 *
 *      Checks if a subgrid is valid (a subgrid being a 3x3 inside main board)
 *      It checks if all 9 subgrids have any duplicates 
 *
 * Parameters:
 *      UArray2_T array: representing our sudoku board
 * Return: 
 *      int representing its validity (1 is valid)
 * Expects
 *      None
 * Notes:
 *      Helper functions may call CRE if invalid numbers in input
 ************************/
int valid_subgrid(T array) {
        int subgrid[DIMENSIONS];

        for (int row = 0; row < DIMENSIONS; row += 3) {
                for (int column = 0; column < DIMENSIONS; column += 3) {
                        get_subgrid(array, subgrid, column, row);
                        if (!is_valid(subgrid)) {
                                return 0; 
                        }
                }
        }
        return 1; 
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
T read_pgm(FILE *file)
{
        assert(file != NULL);
        assert(!ferror(file));

        char magic_number[3];
        int width, height, maxval;

        /* Read and check magic number */
        fscanf(file, "%2s", magic_number);
        assert(magic_number[0] == 'P' && magic_number[1] == '2');

        // skip_comments(file);

        /* Read and check proportions */
        fscanf(file, "%d %d", &width, &height);
        assert(width == 9 && height == 9);
        T table = UArray2_new(width, height, SIZE);
        
        //maybe add thing to skip over description
        fscanf(file, "%d", &maxval);
        assert(maxval == 9);

        for (int i = 0; i < DIMENSIONS; i++) {
                for (int j = 0; j < DIMENSIONS; j++) {
                       int value;
                       int readIn = fscanf(file, "%d", &value);
                       // CHECK IF START AT 0 OR 1
                       assert(readIn == 1 && value > 0 && value <= 9);
                       *(int *)UArray2_at(table, j, i) = value;
                }
        }
        assert(UArray2_height(table) == 9);
        assert(UArray2_width(table) == 9);

        return table;
}

