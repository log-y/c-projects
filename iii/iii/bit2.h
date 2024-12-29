/**************************************************************
 *
 *                     bit2.h
 *
 *     Assignment: iii
 *     Authors:  lyuan04, dperki01, 
 *     Date:     9/25/24
 *
 *      Header file for a 2d array of Bits using Bit_T. 
 * 
 *      Contains the functions that can create, edit, and free a Bit2.
 *     
 **************************************************************/

#ifndef BIT2_H
#define BIT2_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "bit2.h"

#define T Bit2_T
typedef struct T *T;


/********** Bit2_new ********
 *
 * Creates a new instance of the Bit2_T struct.
 *
 * Parameters:
 *      int width: intended width of the array
 *      int height: intended height of the array
 * Return: 
 *      A pointer to a new instance of a Bit2_T
 * Expects
 *      width, height, and size are not NULL
 * Notes:
 *      Will CRE if width or height are NULL
 *      May CRE if malloc fails.
 ************************/
T Bit2_new(int width, int height);

/********** Bit2_width ********
 *
 * Returns the width of the 2D Array element in the inputed Bit2_T struct
 *
 * Parameters:
 *      Bit2_T *array: a pointer to a Bit2_T struct
 * Return: 
 *      An integer representing the width of the 2D Array element of Bit2_T
 * Expects
 *      The inputted Bit2_T pointer is not NULL
 * Notes:
 *      Will CRE if Bit2_T is NULL
 ************************/
int Bit2_width(T array);

/********** Bit2_T_height ********
 *
 * Returns the height of the 2D Array element in the inputed Bit2_T struct
 *
 * Parameters:
 *      Bit2_T *array: a pointer to a Bit2_T struct
 * Return: 
 *      An integer representing the height of the 2D Array element of Bit2_T
 * Expects
 *      The inputted Bit2_T pointer is not NULL
 * Notes:
 *      Will CRE if Bit2_T is NULL
 ************************/
int Bit2_height(T array);

/********** Bit2_get ********
 *
 * returns 
 *
 * Parameters:
 *      Bit2_T *array: a pointer to a Bit2_T struct
 *      int column: integer representing the column the requested value is in
 *      int row: integer representing the row the requested value is in
 * Return: 
 *      An integer representing the bit value at that index
 * Expects
 *      *array, column, and row are not NULL values
 * Notes:
 *      Will CRE if array, column, or row are NULL
 ************************/
int Bit2_get(T array, int column, int row);

/********** Bit2_put ********
 *
 * Changes the value at that row and column index in inputted Bit2_T array
 *
 * Parameters:
 *      Bit2_T *array: a pointer to a Bit2_T struct
 *      int column: integer representing the column the requested value is in
 *      int row: integer representing the row the requested value is in
 *      int val: int to be placed into array
 * Return: 
 *      Returns previous value
 * Expects
 *      *array, column, row, and marker are not NULL values
 * Notes:
 *      Will CRE if array, column, or row, marker are NULL
 ************************/
int Bit2_put(T array, int column, int row, int val);

/********** Bit2_map_col_major ********
 *
 * Outputs a map of the inputted array with the values in column order, so read
 * starting with all elements in the first column of the 2D Array. ADD TO THIS
 *
 * Parameters:
 *      Bit2_T *array: a pointer to the inputted Bit2_T struct
 *      void *f: WRTE THISE
 *      bool check: a boolean value
 * Return: 
 *      WHATEVER THIS RETURNS
 * Expects
 *      *array, f, and check are not NULL values
 * Notes:
 *      Will CRE if array, f, or check are NULL
 ************************/
void Bit2_map_col_major(T array, 
void (*apply)(int row, int col, T bitArray, int curr, void *p2),  
void* cl);

/********** Bit2_map_row_major ********
 *
 * Outputs a map of the inputted array with the values in row order, so read
 * starting with all elements in the first row of the 2D Array. ADD TO THIS
 *
 * Parameters:
 *      Bit2_T *array: a pointer to the inputted Bit2_T struct
 *      void *f: WRITE THIS
 *      bool check: a boolean value
 * Return: 
 *      WHATEVER THIS RETURNS
 * Expects
 *      *array, f, and check are not NULL values
 * Notes:
 *      Will CRE if array, f, or check are NULL
 ************************/
void Bit2_map_row_major(T array, 
void (*apply)(int row, int col, T bitArray, int curr, void *p2),  
void* cl);

/********** Bit2_free ********
 *
 * Frees all memory used by the inputted Bit2_T
 *
 * Parameters:
 *      Bit2_T *array: a pointer to the inputted Bit2_T struct
 * Return: 
 *      Nothing
 * Expects
 *      *array is not NULL
 * Notes:
 *      Will CRE if array is NULL
 *      May CRE if memory deallocated incorrectly
 ************************/
void Bit2_free(T *array);

#undef T
#endif