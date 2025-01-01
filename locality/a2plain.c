/**************************************************************
 *
 *                     a2plain.c
 *
 *     Assignment: locality
 *     Authors:  lyuan04, cyamam01 
 *     Date:     10/7/24
 *
 *      Declares a number of functions intended to be used on a UArray2.
 *      Exports an inherited method suite from type A2Methods_T.
 *      Depends on our implementation of UArray2_T
 *     
 **************************************************************/

#include <string.h>
#include <a2plain.h>
#include <assert.h>
#include "uarray2.h"

/************************************************/
/* Define a private version of each function in */
/* A2Methods_T that we implement.               */
/************************************************/

/********** new ********
 *
 * Creates a new UArray2 object of type UArray2 (plain, not blocked)
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
static A2Methods_UArray2 new(int width, int height, int size)
{
        return UArray2_new(width, height, (int)size);
}

/********** new_with_blocksize ********
 *
 * returns a new UArray2 (not blocked)
 *
 * Parameters:
 *      width, height, size of block, and size of elements (all as ints)
 * Return: 
 *       The new uarray2 (not blocked)
 * Expects
 *       valid dimensions that are within bounds
 * Notes:
 *      Will CRE if invalid parameters
 *      Client will need to free
 ************************/
static A2Methods_UArray2 new_with_blocksize(int width, int height, int size,
                                            int blocksize)
{
        (void) blocksize;
        return UArray2_new(width, height, size);
}

/********** UArray2b_free********
 *
 * Frees a given UArray2 from memory
 *
 * Parameters:
 *      A2Methods_UArray2: the 2D array to be freed (as a UArray2)
 * Return: 
 *       void
 * Expects
 *       assumes the array2p is of object type UArray2_T
 * Notes:
 *      Will CRE if array2p is null
 ************************/
static void a2free(A2Methods_UArray2 *array2p)
{
        assert(array2p != NULL);
        UArray2_free((UArray2_T *) array2p);
}

/********** width ********
 *
 * Returns width of a 2d array object
 *
 * Parameters:
 *      A2Methods_UArray2: the 2D array that the width is retrieved from
 * Return: 
 *       int representing width
 * Expects
 *       assumes the array2p is of object type UArray2_T
 * Notes:
 *      Will CRE if array2 is null (our UArray2 will handle the CRE)
 ************************/
static int width(A2Methods_UArray2 array2)
{
        return UArray2_width(array2);
}

/********** height ********
 *
 * Returns height of a 2d array object
 *
 * Parameters:
 *      A2Methods_UArray2: the 2D array that the height is retrieved from
 * Return: 
 *       int representing height
 * Expects
 *       assumes the array2p is of object type UArray2_T
 * Notes:
 *      Will CRE if array2 is null (our UArray2 will handle the CRE)
 ************************/
static int height(A2Methods_UArray2 array2)
{
        return UArray2_height(array2);
}

/********** size ********
 *
 * Returns size of a 2d array object
 *
 * Parameters:
 *      A2Methods_UArray2: the 2D array that the size is retrieved from
 * Return: 
 *       int representing size
 * Expects
 *       assumes the array2p is of object type UArray2_T
 * Notes:
 *      Will CRE if array2 is null (our UArray2 will handle the CRE)
 ************************/
static int size(A2Methods_UArray2 array2)
{
        return UArray2_size(array2);
}

/********** blocksize ********
 *
 * Returns blocksize of a 2d array object. Because this file is intended
 * for 2d plains only, this will return 1.
 *
 * Parameters:
 *      A2Methods_UArray2: the 2D array that the blocksize is retrieved from
 * Return: 
 *       int repreesnting blocksize
 * Expects
 *       assumes the array2p is of object type UArray2_T
 * Notes:
 *      None
 ************************/
static int blocksize(A2Methods_UArray2 array2)
{
        (void) array2;
        return 1;
}

/********** at ********
 *
 * Returns element at the position specified
 *
 * Parameters:
 *      A2Methods_UArray2: the 2D array that the width is retrieved from
 * Return: 
 *       A2Methods_Object representing object at the index
 * Expects
 *       assumes the array2 is of object type UArray2_T, Assumes i and j
 *      are valid positions
 * Notes:
 *      Will CRE if array2 is null (our UArray2 will handle the CRE).
 *      Will CRE if coordinates are invalid
 ************************/
static A2Methods_Object *at(A2Methods_UArray2 array2, int i, int j)
{
        return UArray2_at(array2, i, j);
}

typedef void UArray2_applyfun(int i, int j, UArray2_T a2, void *elem, void *cl);

/********** map_row_major ********
 *
 * Maps over the given uarray2 by row major, applies a function to each element
 *
 * Parameters:
 *      A2Methods_UArray2: the 2D array to be mapped
 *      A2Methods_applyfun: a map function containing the coordinates,
 *              the 2d array, the a2methods_object, and a closure
 *      void* cl: the closure containing any helpful parameters we may need
 * Return: 
 *       Void
 * Expects
 *      The apply function is not null
 * Notes:
 *      Will CRE if apply is null
 ************************/
static void map_row_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        assert(apply != NULL);
        UArray2_map_row_major(uarray2, (UArray2_applyfun*)apply, cl);
}

/********** map_row_major ********
 *
 * Maps over the given uarray2 by col major, applies a function to each element
 *
 * Parameters:
 *      A2Methods_UArray2: the 2D array to be mapped
 *      A2Methods_applyfun: a map function containing the coordinates,
 *              the 2d array, the a2methods_object, and a closure
 *      void* cl: the closure containing any helpful parameters we may need
 * Return: 
 *       Void
 * Expects
 *      The apply function is not null
 * Notes:
 *      Will CRE if apply is null
 ************************/
static void map_col_major(A2Methods_UArray2 uarray2,
                          A2Methods_applyfun apply,
                          void *cl)
{
        assert(apply != NULL);
        UArray2_map_col_major(uarray2, (UArray2_applyfun*)apply, cl);
}

struct small_closure {
        A2Methods_smallapplyfun *apply; 
        void                    *cl;
};

/********** apply_small ********
 *
 * An apply function that runs a user-defined function on an element in UArray2
 *
 * Parameters:
 *      int i: the column index of the element
 *      int j: the row index of the element
 *      UArray2_T uarray2: the 2D array being mapped (not used in this function)
 *      void* elem: a pointer to the current element in the 2D array
 *      void* vcl: a closure (small_closure struct) containing the apply 
 *                 function and additional parameters to pass to the apply 
 *                 function
 * Return: 
 *       Void
 * Expects:
 *      - The closure closure isn't null
 *      - The closure contains a valid 'apply' function pointer.
 * 
 * Notes:
 *      - It will CRE if vcl, elem, or closure is null
 ************************/
static void apply_small(int i, int j, UArray2_T uarray2,
                        void *elem, void *vcl)
{
        assert(vcl != NULL);
        struct small_closure *cl = vcl; /* grab closure */
        assert(cl != NULL);
        assert(elem != NULL);
        (void)i;
        (void)j;
        (void)uarray2;
        cl->apply(elem, cl->cl); /* apply closure function */
}

/********** small_map_row_major ********
 *
 * Maps over the UArray2 in row major and applies the apply_small function
 *
 * Parameters:
 *      A2Methods_UArray2 a2: Uarray to be mapped over
 *      A2Methods_smallapplyfun: apply function to use
 *      void* cl: closure that will contain extra info needed
 * Return: 
 *       Void
 * Expects:
 *      - all parameters are not null
 * 
 * Notes:
 *      - It will CRE if any parameters are NULL
 ************************/
static void small_map_row_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        assert(cl != NULL && a2 != NULL && apply != NULL);
        struct small_closure mycl = { apply, cl };
        UArray2_map_row_major(a2, apply_small, &mycl);
}

/********** small_map_col_major ********
 *
 * Maps over the UArray2 in col major and applies the apply_small function
 *
 * Parameters:
 *      A2Methods_UArray2 a2: Uarray to be mapped over
 *      A2Methods_smallapplyfun: apply function to use
 *      void* cl: closure that will contain extra info needed
 * Return: 
 *       Void
 * Expects:
 *      - all parameters are not null
 * 
 * Notes:
 *      - It will CRE if any parameters are NULL
 ************************/
static void small_map_col_major(A2Methods_UArray2        a2,
                                A2Methods_smallapplyfun  apply,
                                void *cl)
{
        assert(cl != NULL && a2 != NULL && apply != NULL);
        struct small_closure mycl = { apply, cl };
        UArray2_map_col_major(a2, apply_small, &mycl);
}

/*
the method suite that we export. this is designed to be used on a plain 2d 
array. Contains NULL for the functions that shouldn't be run on the plain 2d
*/
static struct A2Methods_T uarray2_methods_plain_struct = {
        new,
        new_with_blocksize,
        a2free, 
        width,
        height, 
        size, 
        blocksize,
        at, 
        map_row_major,
        map_col_major,
        NULL,                   /*  map_block_major */
        map_row_major,          /* map_default */
        small_map_row_major,
        small_map_col_major,
        NULL,                   /* small_map_block_major */
        small_map_row_major     /* small_map_default */
};


A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;
