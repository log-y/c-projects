#include "transform_help.h"

int main() 
{
        Op op = ROTATE_0;

        CoordPair pair = {0,0};

        int w = 2;
        int h = 6;
        CoordPair newpair = new_coords(op, pair, w, h);
        printf("rotate 0: %d,%d \n", newpair.row, newpair.col);

        op = ROTATE_90;

        newpair = new_coords(op, pair, w, h);
        printf("rotate 90: %d,%d \n", newpair.row, newpair.col);

        op = ROTATE_180;

        newpair = new_coords(op, pair, w, h);
        printf("rotate 180: %d,%d \n", newpair.row, newpair.col);

        exit(EXIT_SUCCESS);
}

