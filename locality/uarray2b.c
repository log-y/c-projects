/**************************************************************
 *
 *                     uarray2b.c
 *
 *     Assignment: locality
 *     Authors:  lyuan04, cyamam01 
 *     Date:     10/7/24
 *
 *      Initializes the uarray2b struct, giving methods to 
 *      create, access, and free a blocked 2d array. Depends upon
 *      our implementation of UArray2b
 *     
 **************************************************************/


#include "uarray2.h"
#include <uarray.h>
#include <assert.h>
#include <uarray2b.h>


struct UArray2b_T {
        int width;
        int height;
        int size;
        int blocksize;
        UArray2_T data; 
};

#define T struct UArray2b_T *

/********** UArray2b_new********
 *
 * returns a new UArray2b with a specified width, height, size, and 
 * blocksize
 *
 * Parameters:
 *      width, height, blocksize, and size of elements (all as ints)
 * Return: 
 *       The new uarray2b
 * Expects
 *       valid dimensions that are within bounds
 * Notes:
 *      Will CRE if invalid width, height, size, blocksize.
 *      Will also CRE if can't allocate the memory
 *      Client will need to free
 ************************/
T UArray2b_new(int width, int height, int size, int blocksize)
{
        if(width < 1 || height < 1 || size < 1 || blocksize < 1) {
                fprintf(stderr, "bad array dimensions\n");
                exit(1);
        }
        
        int width_in_blocks = width / blocksize + 1;
        int height_in_blocks = height / blocksize + 1;

        T array_blocks = malloc(sizeof(*array_blocks)); /*memory alloc*/
        if (array_blocks == NULL) {
                fprintf(stderr, "unable to allocate memory for array\n");
                exit(1);
        }

        /*memory for 2d array*/
        UArray2_T blocks = UArray2_new(width_in_blocks, 
                                        height_in_blocks, 
                                        sizeof(UArray_T));
        if (blocks == NULL) {
                fprintf(stderr, "unable to allocate memory for array\n");
                exit(1);
        }

        /* initialize dimensions */
        array_blocks->width = width;
        array_blocks->height = height;
        array_blocks->size = size;
        array_blocks->blocksize = blocksize;
        array_blocks->data = blocks;

        int elements_in_block = blocksize * blocksize;

        /* initialize each block*/
        for(int row = 0; row < height_in_blocks; row++){
                for(int col = 0; col < width_in_blocks; col++){
                        UArray_T block = UArray_new(elements_in_block, size);
                        assert(block != NULL);

                        *(UArray_T *)UArray2_at(blocks, col, row) = block;
                }
        }

        return array_blocks;

}

/********** UArray2b_new_64k_block********
 *
 * returns a new UArray2b with each block being size 64k
 *
 * Parameters:
 *      width, height, and size of elements (all as ints)
 * Return: 
 *       The new uarray2b
 * Expects
 *       valid dimensions (within bounds)
 * Notes:
 *      Will CRE if invalid parameters
 *      Client will need to free
 ************************/
T UArray2b_new_64K_block(int width, int height, int size)
{
        assert(width >= 1 && height >= 1 && size >= 1 && size >= 1);

        T new_64k_block = UArray2b_new(width, height, size, 256);
        assert(new_64k_block != NULL);
        return new_64k_block;
}

/********** UArray2b_free********
 *
 * Frees a given UArray2b from memory
 *
 * Parameters:
 *      T array2b: the 2D blocked array 
 * Return: 
 *       void
 * Expects
 *       a valid input array
 * Notes:
 *      Will CRE if array2b is null, or if any blocks are null
 ************************/
void UArray2b_free (T *array2b)
{
        assert(array2b != NULL);
        int numBlocks = (*array2b)->blocksize;
        UArray2_T blocks = (*array2b)->data;
        assert(blocks != NULL);
        int height = (*array2b)->height;
        int width = (*array2b)->width;
        /* frees each block */
        for(int row = 0; row < height / numBlocks + 1; row++){
                for(int col = 0; col < width / numBlocks + 1; col++){
                        UArray_T* elem = UArray2_at(blocks, col, row);
                        assert(elem != NULL);
                        UArray_free(elem);
                }
        }
        UArray2_free(&blocks);
        free(*array2b); 
}

/********** UArray2b_width ********
 *
 * Returns the width of a given UArray2b
 *
 * Parameters:
 *      T array2b: the 2D blocked array to access
 * Return: 
 *       int*: width as an int
 * Expects
 *       a valid input array
 * Notes:
 *      Will CRE if array2b is null
 ************************/
int UArray2b_width (T array2b)
{
        assert(array2b != NULL);
        return array2b->width;
}

/********** UArray2b_height********
 *
 * Returns the height of a given UArray2b
 *
 * Parameters:
 *      T array2b: the 2D blocked array to access
 * Return: 
 *       int*: height as an int
 * Expects
 *       a valid input array
 * Notes:
 *      Will CRE if array2b is null
 ************************/
int UArray2b_height (T array2b)
{
        assert(array2b != NULL);
        return array2b->height;
}

/********** UArray2b_size ********
 *
 * Returns the size of an element in a given UArray2b
 *
 * Parameters:
 *      T array2b: the 2D blocked array to access
 * Return: 
 *       int*: size as an int
 * Expects
 *       a valid input array
 * Notes:
 *      Will CRE if array2b is null
 ************************/
int UArray2b_size (T array2b)
{
        assert(array2b != NULL);
        return array2b->size;
}

/********** UArray2b_blocksize ********
 *
 * Returns the blocksize of a given UArray2b
 *
 * Parameters:
 *      T array2b: the 2D blocked array to access
 * Return: 
 *       int*: blocksize as an int
 * Expects
 *       a valid input array
 * Notes:
 *      Will CRE if array2b is null
 ************************/
int UArray2b_blocksize(T array2b)
{
        assert(array2b != NULL);
        return array2b->blocksize;
}

/********** UArray2b_at ********
 *
 * retrieves a pointer to an element in a 2D blocked array at the given
 * column and row
 *
 * Parameters:
 *      T array2b: the 2D blocked array to access
 *      int column: the column index of the desired element
 *      int row: the row index of the desired element
 * Return: 
 *       void*: a pointer to the element at the specified position
 * Expects
 *       array2b is valid, and column/row indices are within bounds
 * Notes:
 *      Will CRE if array2b, data, or block is null, or if column/row not
 *      in bounds
 ************************/
void *UArray2b_at(T array2b, int column, int row)
{
        assert(array2b != NULL);
        int width = array2b->width;
        int height = array2b->height;
        int blocksize = array2b->blocksize;
        assert(column >= 0 && column < width);
        assert(row >= 0 && row < height);
        UArray2_T data = array2b->data;
        assert(data != NULL);

        UArray_T* block = UArray2_at(data, column / blocksize, row / blocksize);
        assert(block != NULL);
        void* element = UArray_at(*block, 
                                  blocksize * (row % blocksize) + 
                                  (column % blocksize));
        return element;
}

/********** UArray2b_map ********
 *
 * Applies a given function to each element block-by-block
 *
 * Parameters:
 *      T array2b: the 2D blocked array to traverse
 *      void apply(int col, int row, T array2b, void *elem, void *cl) apply fun
 *      void *cl: closure pointer, passed to the apply function
 * Return: 
 *      void
 * Expects
 *       array2b and the apply function are valid
 * Notes:
 *      will CRE if array2b or any block is null
 ************************/
void UArray2b_map(
T array2b,
void apply(int col, int row, T array2b, void *elem, void *cl),
void *cl) 
{
        assert(array2b != NULL);

        int width = array2b->width;
        int height = array2b->height;
        int bsz = array2b->blocksize;

        UArray2_T data = array2b->data;

        /* go over each block */
        for (int b_row = 0; b_row < (height + bsz - 1) / bsz; b_row++) {
                for (int b_col = 0; b_col < (width + bsz - 1) / bsz; b_col++) {
                /*grab the block*/
                UArray_T* b = UArray2_at(data, b_col, b_row);
                assert(b != NULL);
                /* go through each block */
                for (int i = 0; i < bsz; i++) {
                        for (int j = 0; j < bsz; j++) {
                        /* get the position in the actual array*/
                                int g_row = b_row * bsz + i;
                                int g_col = b_col * bsz + j;
                                /* check bounds */
                                if (g_row < height && g_col < width) {
                                        void *ele = UArray_at(*b, i * bsz + j);
                                        assert(ele != NULL);
                                        /* run ur apply function */
                                        apply(g_col, g_row, array2b, ele, cl);
                                }
                        }
                }
                }
        }
}
#undef T


