/**************************************************************
 *
 *                     ppmtrans.c
 *
 *     Assignment: locality
 *     Authors:  lyuan04, cyamam01 
 *     Date:     10/7/24
 *
 *      Reads in a file with a given rotation and major order, then
 *      rotates the file and prints it to stdout. Relies on our 
 *      implementation of uarray2b and uses the A2Method_T suite defined
 *      in /comp/40/build/include/a2methods.h
 *     
 **************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "transform_help.h"
#include "cputiming.h"

/* keeps track of what major we need to use */
typedef enum { 
        ROW_MAJ,
        COL_MAJ,
        BLOCK_MAJ
} Major;

/* macro to set our methods, makes our main function look nicer */
#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (false)

char *get_transformation_name(Op operation);
char *get_maj_name(Major m);
void get_dimensions(Op trf, CoordPair* new_dims, Pnm_ppm original_img);
void initialize_new_image(Pnm_ppm new_img, CoordPair new_dims, 
                                Pnm_ppm original_img, A2Methods_T methods);
void handle_timing(FILE* timefile, char* infile_name, double time_used, 
                        Pnm_ppm new_image, Op trf, Major maj);

/* usage error message */
static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] "
                        "[-time time_file] "
                        "[filename]\n",
                        progname);
        exit(1);
}

/********** main ********
 *
 * Reads the commands from the command line, then calls the appropriate 
 * functions to execute them
 *
 * Parameters:
 *      int argc: number of cli arguments
 *      char* argv[]: array of each element
 * Return: 
 *      int representing if execution happened correctly
 * Expects
 *      valid command line arguments
 * Notes:
 *      Will exit if command line isn't accurate, will CRE if files open
 *      improperly
 ************************/
int main(int argc, char *argv[])
{
        char *time_file_name = NULL; /* initialize all of our variables */
        char *infile_name = NULL;
        int   rotation       = 0;
        int   i;
        double time_used;
        bool timing = false;
        FILE *infile = NULL; /* needs to be initialized to NULL otherwise */
        FILE *timefile = NULL; /* we get valgrind errors */
        CPUTime_T timer = CPUTime_New();
        Op trf = ROTATE_0; /* will store the operation we need to calculate*/
        Major maj = ROW_MAJ; /* will store the type of major needed */

        /* default to UArray2 methods */
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods != NULL);

        /* default to best map */
        A2Methods_mapfun *map = methods->map_default; 
        assert(map != NULL);

        /* parse arguments into our variables */
        for (i = 1; i < argc; i++) {
                /* if -row major is an argument */
                if (strcmp(argv[i], "-row-major") == 0) {
                        /* set methods to map row-major */
                        SET_METHODS(uarray2_methods_plain, map_row_major, 
                                    "row-major");
                        map = methods->map_row_major;
                        maj = ROW_MAJ;
                } else if (strcmp(argv[i], "-col-major") == 0) {
                        SET_METHODS(uarray2_methods_plain, map_col_major, 
                                    "column-major");
                        map = methods->map_col_major;
                        maj = COL_MAJ;
                } else if (strcmp(argv[i], "-block-major") == 0) {
                        SET_METHODS(uarray2_methods_blocked, map_block_major,
                                    "block-major");
                        map = methods->map_block_major;
                        maj = BLOCK_MAJ;
                } else if (strcmp(argv[i], "-rotate") == 0) {
                        if (!(i + 1 < argc)) {  /* no rotate value */
                                usage(argv[0]);
                        }
                        /* check for errors in strtol */
                        char *endptr;
                        rotation = strtol(argv[++i], &endptr, 10);
                        if (!(rotation == 0 || rotation == 90 ||
                            rotation == 180 || rotation == 270)) {
                                fprintf(stderr, 
                                        "Rotation must be 0, 90 180 or 270\n");
                                usage(argv[0]);
                        }
                        if (!(*endptr == '\0')) {    /* Not a number */
                                usage(argv[0]);
                        }
                        switch (rotation) {
                                case 0: 
                                        trf = ROTATE_0;
                                        break;
                                case 90: 
                                        trf = ROTATE_90;
                                        break;
                                case 180: 
                                        trf = ROTATE_180;
                                        break;
                                case 270: 
                                        trf = ROTATE_270;
                                        break;
                                default: 
                                        trf = ROTATE_0;
                                        break;
                                        
                        }
                } else if (strcmp(argv[i], "-flip") == 0) {
                        if (strcmp(argv[++i], "horizontal") == 0) {
                                trf = FLIP_H;
                        } else if (strcmp(argv[i], "vertical") == 0) {
                                trf = FLIP_V;
                        } else {
                                fprintf(stderr, 
                                        "flip must be horizontal "
                                        "or vertical\n");
                                usage(argv[0]);
                        }
                } else if (strcmp(argv[i], "-transpose") == 0) {
                        trf = TRANSPOSE;
                } else if (strcmp(argv[i], "-time") == 0) {
                        if (!(i + 1 < argc)) {      /* no time file */
                                usage(argv[0]);
                        }
                        time_file_name = argv[++i];
                        timefile = fopen(time_file_name, "a");
                        if (timefile == NULL) {
                                fprintf(stderr, "bad time file\n");
                                exit(1);
                        }
                        timing = true;
                } else if (*argv[i] == '-') {
                        fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                                argv[i]);
                        usage(argv[0]);
                } else if (argc - i > 1) {
                        fprintf(stderr, "Too many arguments\n");
                        usage(argv[0]);
                } else {
                        infile_name = argv[i];
                        infile = fopen(argv[i], "rb");
                        if (infile == NULL) {
                                fprintf(stderr, "bad file input\n");
                                exit(1);
                        }
                        break;
                }
        }
        /* check if we need to read from stdin or a file */
        if (infile == NULL) {
                infile = stdin;
                infile_name = "from stdin";
        }

        Pnm_ppm original_img = Pnm_ppmread(infile, methods); /* read file */
        assert(original_img != NULL);

        fclose(infile);

        CoordPair new_dims; /* store the new dimensions */
        get_dimensions(trf, &new_dims, original_img); /* grab the new dims*/

        /* create new image */
        Pnm_ppm new_img = malloc(sizeof(*new_img)); 
        assert(new_img != NULL);

        /* initialize the new image based on the new dimensions*/
        initialize_new_image(new_img, new_dims, original_img, methods);

        /* will store the needed parameters we need for transformation */
        Close_struct cl = make_close(&(new_img->pixels), trf, methods);
        assert(cl != NULL);

        CPUTime_Start(timer); /* start timing */
        map(original_img->pixels, apply_t, cl); /* run the actual mapping */
        time_used = CPUTime_Stop(timer); /* stop timing */

        /* if we need to keep track of timing, write the timing information 
        to the file specified, and append a column to the csv */
        if (timing) { 
                handle_timing(timefile, infile_name, time_used, 
                        new_img, trf, maj);
        } else {
                (void) time_used;
        }

        Pnm_ppmwrite(stdout, new_img); /* write to stdour using pnm.h */

        Pnm_ppmfree(&original_img); /* make sure we free all the images */
        Pnm_ppmfree(&new_img); 
        free(cl);
        if(timing){ /* and close all of the files */
                fclose(timefile);
        }
        CPUTime_Free(&timer);
        return 0;
}

/********** handle_timing ********
 *
 * Handles the writing to the timing file, and also writes to a 
 * .csv file
 *
 * Parameters:
 *      FILE* timefile: represents file that we write timing info to
 *      char* infile_name: represents the name of the file we need to write
 *      double time_used: responsible to storing how long operation took
 *      Pnm_ppm new_img: represents the new image (used for dimensions)
 *      Op trf: represents the operation we ran
 *      Major maj: represents the major order we ran
 * Return: 
 *      void
 * Expects
 *      timefile and new_img are valid;
 * Notes:
 *      Will CRE if timefile is null or new_img is null
 ************************/
void handle_timing(FILE* timefile, char* infile_name, double time_used, 
                        Pnm_ppm new_img, Op trf, Major maj){
        FILE* csvfile = NULL;
        assert(timefile != NULL && new_img != NULL);
        /*write the relevant information to the file specified on cmd */
        fprintf(timefile, "image %s was %s using %s " 
                        "mapping in %.6f milliseconds "
                        "with %.6f milliseconds per pixel\n", 
                        infile_name, 
                        get_transformation_name(trf), 
                        get_maj_name(maj),
                        time_used / 1000000,
                        time_used / 1000000 / 
                        (new_img->width * new_img->height));
        csvfile = fopen("time_table.csv", "a");
        if (csvfile == NULL) {
                fprintf(stderr, "you did a bad somehow\n");
                exit(1);
        }
        /* 
        * csvfile is a .csv file that we are using to make
        * a table/spreadsheet from the data we create
        * 
        * the header on the file is 
        *      Image Name,Transformation,Mapping Type,Time(ns)
        */
        fprintf(csvfile, "%s,%s,%s,%.0f\n", 
                        infile_name, 
                        get_transformation_name(trf), 
                        get_maj_name(maj),
                        time_used);
        fclose(csvfile);
}

/********** initialize_new_image ********
 *
 * Initializes a new image with correct dimensions, provides a beginning
 * pixel array
 *
 * Parameters:
 *      Pnm_ppm new_img: the new image to initialize
 *      CoordPair new_dims: holds the new dimensions (width, height)
 *      Pnm_ppm original_img: the original image, used to copy denominator
 *      A2Methods_T methods: methods for 2D array operations
 * Return: 
 *      void
 * Expects
 *       new_img, original_img, and methods are valid
 * Notes:
 *       Will CRE if new_img, original_img, or methods are null
 *      new_img->pixels will need to be freed by client
 ************************/
void initialize_new_image(Pnm_ppm new_img, CoordPair new_dims, 
                                Pnm_ppm original_img, A2Methods_T methods){
        assert(new_img != NULL && original_img != NULL && methods != NULL);
        new_img->width = new_dims.col;
        new_img->height = new_dims.row;
        new_img->denominator = original_img->denominator;
        new_img->methods = methods;
        new_img->pixels = methods->new(new_img->width, new_img->height, 
                                       sizeof(struct Pnm_rgb));
}

/**********  get_dimensions ********
 *
 * calculates the dimensions for a new image based on the operation
 *  and the dimensions of the original image.
 *
 * Parameters:
 *      Op trf: the transformation operation to perform
 *      CoordPair* new_dims: stores the calculated dimensions (width, height)
 *      Pnm_ppm original_img: the original image to get current dimensions
 * Return: 
 *      void
 * Expects
 *       new_dims and original_img are valid
 * Notes:
 *       Will CRE if new_dims or original_img are null
 ************************/
void get_dimensions(Op trf, CoordPair* new_dims, Pnm_ppm original_img){
        assert(original_img != NULL);
        assert(new_dims != NULL);
        switch (trf) { /* check for all possibilities*/
                case ROTATE_90: 
                case ROTATE_270:
                case TRANSPOSE: 
                        new_dims->col = original_img->height;
                        new_dims->row = original_img->width;
                        break;
                case ROTATE_0:
                case ROTATE_180: 
                case FLIP_H:
                case FLIP_V: 
                default:
                        new_dims->col = original_img->width;
                        new_dims->row = original_img->height;
                        break;
                
        }
}

/**********  get_transformation_name ********
 *
 * returns a string that represents the name of the operation performed
 * based on the transformation type.
 *
 * Parameters:
 *      Op operation: the transformation operation to get the name for
 *      char*: a string describing the transformation
 *      Pnm_ppm original_img: the original image to get current dimensions
 * Return: 
 *      char* representing name of transformation
 * Expects
 *       operation is a valid transformation enum
 * Notes:
 *       returns "booped on the nose" for any unrecognized operation
 ************************/
char *get_transformation_name(Op operation)
{
        switch (operation) {
        case ROTATE_0:
                return "rotated 0 degrees";
                break;
        case ROTATE_90:
                return "rotated 90 degrees";
                break;
        case ROTATE_180:
                return "rotated 180 degrees";
                break;
        case ROTATE_270:
                return "rotated 270 degrees";
                break;
        case FLIP_H:
                return "flipped horizontally";
                break;
        case FLIP_V:
                return "flipped vertically";
                break;
        case TRANSPOSE:
                return "transposed";
                break;
        default:
                return "booped on the nose";
                break;
        }
}

/**********  get_maj_name ********
 *
 * returns a string that represents the name of the major order used
 * for mapping.
 *
 * Parameters:
 *      Major m: the major order type to get the name for
 * Return: 
 *       char*: a string describing the major order
 * Expects
 *        m is a valid major order enum
 * Notes:
 *       returns "no mapping at all????" for any unrecognized major order
 ************************/
char *get_maj_name(Major m) 
{
        switch (m) {
                case ROW_MAJ:
                        return "row-major";
                        break;
                case COL_MAJ:
                        return "column-major";
                        break;
                case BLOCK_MAJ:
                        return "block-major";
                        break;
                default:
                        return "no mapping at all????";
                        break;
        }
}

