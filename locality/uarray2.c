/**************************************************************
 *
 *                     uarray2.c
 *
 *     Assignment: iii
 *     Authors:  lyuan04, dperki01, 
 *     Date:     9/25/24
 *
 *      Implements a 2d array using Hanson UArray. It places the rows 
 *      all in one single 1d array, and uses multiplication/addition to access 
 *      specific elements
 * 
 *      Our central data structure is a Hanson UArray, which we use to store 
 *      all of our data contiguously. The get and put functionality are all
 *      O(1).
 *     
 **************************************************************/


#include "uarray2.h"
#include <assert.h>
#include <uarray.h>

#define T UArray2_T

struct UArray2_T {
    int width;
    int height;
    int size;
    UArray_T data;
};

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
T UArray2_new(int width, int height, int size){
        if(!(width >= 1 && height >= 1 && size >= 1)){
                fprintf(stderr, "bad array dimensions\n");
                exit(1);
        }

        UArray_T newData = UArray_new(width * height, size);
        assert(newData != NULL);

        T array2 = malloc(sizeof(*array2));
        assert(array2 != NULL);
        array2->width = width;
        array2->height = height;
        array2->size = size;
        array2->data = newData;
        return array2;
}

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
int UArray2_width(T array){
        assert(array != NULL);
        return array->width;
}

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
int UArray2_height(T array){
        assert(array != NULL);
        return array->height;
}

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
int UArray2_size(T array){
        assert(array != NULL);
        return array->size;
}

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
void *UArray2_at(T array, int column, int row){
        assert(array != NULL);
        UArray_T data = array->data;
        int width = array->width;
        assert(column >= 0 && column < width);
        assert(row >= 0 && row < array->height);
        void* element = UArray_at(data, width * row + column);
        return element;
}

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
void* cl){
        assert(array != NULL);
        int width = array->width;
        int height = array->height;
        UArray_T data = array->data;

        for(int i = 0; i < width; i++){
                for(int j = 0; j < height; j++){
                        void* elem = UArray_at(data, i + j * width);
                        apply(i, j, array, elem, cl);
                }
        }
}

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
void* cl){
        assert(array != NULL);
        int width = array->width;
        int height = array->height;
        UArray_T data = array->data;

        for(int i = 0; i < height; i++){
                for(int j = 0; j < width; j++){
                        void* elem = UArray_at(data, i * width + j);
                        apply(j, i, array, elem, cl);
                }
        }
}

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
void UArray2_free(T *array){
        assert(array != NULL);
        UArray_T data = (*array)->data;
        UArray_free(&data);
        free(*array);
}

#undef T