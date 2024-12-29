/**************************************************************
 *
 *                     uarray2.h
 *
 *     Assignment: iii
 *     Authors:  lyuan04, dperki01 
 *     Date:     9/20/24
 *
 *     Created a 2d array using uarrays. This 2d array can fit any data type
 *     and can be initialized to any dimensions MxN. This file defines the
 *     file headers for all of the exported functions that will create, 
 *     access, and free (etc) a uarray2.
 *
 **************************************************************/
#ifndef UARRAY2_H
#define UARRAY2_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define T UArray2_T
typedef struct T *T;

/********** UArray2_new ********
 *
 * Constructs a UArray2 instance. Initializes the dimensions and data type.
 *
 * Parameters:
 *      int width: represents how many columns needed
 *      int height: represents how many rows needed
 *      int size: size of each element in bytes 
 * Return: 
 *      UArray2_T* (a pointer to the new object)
 * Expects
 *      valid width, height, and size
 * Notes:
 *      Will CRE if can't find memory for construction
 ************************/
T UArray2_new( int width, int height, int size);

/********** UArray2_width ********
 *
 * Returns the width of the 2D Array element in the inputed UArray2_T struct
 *
 * Parameters:
 *      UArray2_T *array: a pointer to a UArray2_T struct
 * Return: 
 *      An integer representing the width of the 2D Array element of UArray2_T
 * Expects
 *      The inputted UArray2_T pointer is not NULL
 * Notes:
 *      Will CRE if UArray2_T is NULL
 ************************/
int UArray2_width(T array);

/********** UArray2_height ********
 *
 * Returns the height of the 2D Array element in the inputed UArray2_T struct
 *
 * Parameters:
 *      UArray2_T *array: a pointer to a UArray2_T struct
 * Return: 
 *      An integer representing the height of the 2D Array element of UArray2_T
 * Expects
 *      The inputted UArray2_T pointer is not NULL
 * Notes:
 *      Will CRE if UArray2_T is NULL
 ************************/
int UArray2_height(T array);

/********** UArray2_size ********
 *
 * Returns the size of the 2D Array element in the inputed UArray2_T struct
 *
 * Parameters:
 *      UArray2_T *array: a pointer to a UArray2_T struct
 * Return: 
 *      An integer representing the size of the 2D Array element of UArray2_T
 * Expects
 *      The inputted UArray2_T pointer is not NULL
 * Notes:
 *      Will CRE if UArray2_T is NULL
 ************************/
int UArray2_size(T array);

/********** UArray2_at ********
 *
 * returns the element at the specified index. this will return a void pointer
 *
 * Parameters:
 *      UArray2_T *array: a pointer to a UArray2_T struct
 *      int column: integer representing the column the requested value is in
 *      int row: integer representing the row the requested value is in
 * Return: 
 *      A void pointer to the specified index in the array
 * Expects
 *      *array is not NULL, col and row are valid indexes
 * Notes:
 *      Will CRE if array, column, or row are NULL
 *      May CRE if malloc fails.
 ************************/
void *UArray2_at(T array, int column, int row);

/********** UArray2_map_col_major ********
 *
 * Iterates through our 2d array in column major traversal. Applies a 
 function to each element. 
 *
 * Parameters:
 *      UArray2_T *array (typedefed to T): a pointer to a struct UArray2
 *      void *f: a pointer to a function
            int row: row index of element
            int col: col index of element
            T currArray: pointer to array being traversed
            void* curr: pointer to current element
            void *p2: closure pointer passed to function
 *      void* cl: additional context , tells user if array is okay
 * Return: 
 *      None
 * Expects
 *      array is not null. for the inner apply function, expects that
        col and row are valid indexes, that curArray is not null 
 * Notes:
 *      Will CRE if array, apply is null, f, or check are NULL
 ************************/
void UArray2_map_col_major(T array, 
void (*apply)(int row, int col, T currArray, void *curr, void *p2),  
void* cl);

/********** UArray2_map_row_major ********
 *
 * Iterates through our 2d array in row major traversal. Applies a 
 function to each element. 
 *
 * Parameters:
 *      UArray2_T *array (typedefed to T): a pointer to a struct UArray2
 *      void *f: a pointer to a function
            int row: index of current element's row 
            int col: index of current element's col
            T currArray: pointer to array being traversed
            void* curr: pointer to current element
            void *p2: closure pointer passed to function
 *      void* cl: context pointer, tells user if array is okay
 * Return: 
 *      None
 * Expects
 *      array is not null. for the inner apply function, expects that
        col and row are valid indexes, also that curArray is not null 
 * Notes:
 *      Will CRE if array, f, or check are NULL
 ************************/
void UArray2_map_row_major(T array, 
void (*apply)(int row, int col, T currArray, void *curr, void *p2),  
void* cl);

/********** UArray2_free ********
 *
 * Frees all memory used by the inputted UArray2_T
 *
 * Parameters:
 *      UArray2_T *array: a pointer to the inputted UArray2_T struct
 * Return: 
 *      Nothing
 * Expects
 *      *array is not NULL
 * Notes:
 *      Will CRE if array is NULL
 *      May CRE if memory deallocated incorrectly
 ************************/
void UArray2_free(T *array);


#undef T

#endif 