/**************************************************************
 *
 *                     transform_help.h
 *
 *     Assignment: locality
 *     Authors:  lyuan04, cyamam01 
 *     Date:     10/7/24
 *
 *      Implements a number of helper functions we will use in our rotations.
 *      This will help us apply functions and calculate locations for new
 *      coordinates.
 *     
 **************************************************************/

#include "transform_help.h"
#include <assert.h>

/********** new_coords ********
 *
 * Takes in a pair of coordinates and calculates its new location given the
 * specified operation (rotation)
 *
 * Parameters:
 *      Op tm: specified rotation
 *      CoordPair orig: original coordinates
 *      int orig_w: original width
 *      int orig_h: original height
 * Return: 
 *      Coordpair representing new coordinates
 * Expects
 *      Valid orig_w, orig_h, operation
 * Notes:
 *      Will Exit if an invalid rotation is given
 ************************/
CoordPair new_coords(Op tm, CoordPair orig, int orig_w, int orig_h)
{
        CoordPair new_cs;
        switch (tm) {
                case ROTATE_0: 
                        return orig; 
                        break;
                case ROTATE_90: 
                        new_cs = make_coords(orig_h - orig.row - 1, orig.col);
                        return new_cs;
                        break;
                case ROTATE_180: 
                        new_cs = make_coords(orig_w - orig.col - 1, 
                                             orig_h - orig.row - 1);
                        return new_cs;
                        break;
                case ROTATE_270: 
                        new_cs = make_coords(orig.row, orig_w - orig.col - 1);
                        return new_cs;
                        break;
                case FLIP_H: 
                        new_cs = make_coords(orig_w - orig.col - 1, orig.row);
                        return new_cs;
                        break;
                case FLIP_V: 
                        new_cs = make_coords(orig.col, orig_h - orig.row - 1);
                        return new_cs;
                        break;
                case TRANSPOSE: 
                        new_cs = make_coords(orig.row, orig.col);
                        return new_cs;
                        break;
                default: 
                        fprintf(stderr, "you should not see this\n");
                        exit(1);
        }
}

/********** apply_t ********
 *
 * Applies the rotation to a pixel. Moves information from the input pixel
 * to our output image
 *
 * Parameters:
 *      int col: column of current pixel
 *      int row: row of current pixel
 *      A2Methods_UArray2 array: current input array read from pnm.h 
 *      void* elem: current element
 *      void* cl: closure containing helper attributes
 * Return: 
 *      void
 * Expects
 *      valid input and output UArray2s
 * Notes:
 *      Will CRE if invalid input/output UArray
 ************************/
void apply_t(int col, int row, A2Methods_UArray2 array, void *elem, void *cl)
{
        if (cl == NULL) {
                fprintf(stderr, "don't give me a null closure :(\n");
                exit(1);
        }
        if (elem == NULL) {
                fprintf(stderr, "null element >:(\n");
                exit(1);
        }
        Close_struct my_cl = cl;
        assert(my_cl != NULL);
        A2Methods_T methods = my_cl->methods;
        A2Methods_UArray2 other_array = *(my_cl->array);
        assert(other_array != NULL && methods != NULL);
        CoordPair orig_coords = make_coords(col, row);
        
        CoordPair n_coords = new_coords(my_cl->tm, orig_coords, 
                                        methods->width(array), 
                                        methods->height(array));

        A2Methods_Object *orig_obj = elem;
        
        stored *value = methods->at(other_array, n_coords.col, n_coords.row);
        assert(value != NULL);
        *value = *((stored *)(orig_obj));
}

/********** make_coords ********
 *
 * Creates a CoordPair given a column and row
 *
 * Parameters:
 *      int col: column of pixel
 *      int row: row of pixel
 * Return: 
 *      Coordpair representing the coordinates as a struct
 * Expects
 *      None
 * Notes:
 *      None
 ************************/
CoordPair make_coords(int col, int row)
{
        CoordPair pair;
        pair.col = col;
        pair.row = row;
        return pair;
}

/********** make_close ********
 *
 * Makes our closure struct from an output array, rotation, and methods
 *
 * Parameters:
 *      A2Methods_UArray2 *array: the image we are writing to
 *      Op tm: the operation we need to do
 *      A2Methods_T methods: the methods we are allowed to use
 * Return: 
 *      Close_struct representing our information wrapped in a struct
 * Expects
 *      None
 * Notes:
 *      None
 ************************/
Close_struct make_close(A2Methods_UArray2 *array, Op tm, A2Methods_T methods)
{
        Close_struct close = malloc(sizeof(*close));
        assert(close != NULL);
        close->array = array;
        close->tm = tm;
        close->methods = methods;
        return close;
}
