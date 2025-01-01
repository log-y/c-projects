/**************************************************************
 *
 *                     transform_help.c
 *
 *     Assignment: locality
 *     Authors:  lyuan04, cyamam01 
 *     Date:     10/7/24
 *
 *      Defines a number of helper functions we will use in our rotations.
 *      This will help us apply functions and calculate locations for new
 *      coordinates.
 *     
 **************************************************************/

#ifndef TRANS_HELP_H
#define TRANS_HELP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "a2methods.h"
#include "pnm.h"
#include "a2plain.h"
#include "a2blocked.h"

typedef enum {
        ROTATE_0,
        ROTATE_90,
        ROTATE_180,
        ROTATE_270,
        FLIP_H,
        FLIP_V,
        TRANSPOSE
} Op;

typedef struct {
        int col, row;
} CoordPair;

typedef struct {
        A2Methods_UArray2 *array;
        Op tm;
        A2Methods_T methods;
} *Close_struct;

/* used for efficiency, can be changed to any type */
typedef struct Pnm_rgb stored;

CoordPair new_coords(Op tm, CoordPair orig, int orig_w, int orig_h);
void apply_t(int col, int row, A2Methods_UArray2 array, void *elem, void *cl);

CoordPair make_coords(int col, int row);
Close_struct make_close(A2Methods_UArray2 *array, Op tm, A2Methods_T methods);

#endif