/**************************************************************
 *
 *                     restoration.c
 *
 *     Assignment: filesofpix
 *     Authors:  lyuan04, dperki01, 
 *     Date:     9/15/24
 *
 *      This file is the main file of the program. It is the central file
 *      that calls the functions in other files.
 *      The purpose of this program is to 1) restore a corrupted image 
 *      given in p2 format and 2) convert it to raw p5 format.
 * 
 *      Our central data structure is a Hanson table, which we use to check 
 *      for matching character infusion sequences. We also use a Hanson seq
 *      to hold all of the pixel rows for the corrected p5 image.
 *     
 *
 **************************************************************/


#include "readaline.h"
#include "processLine.c"
#include "freeMemory.c"
#include <stdlib.h>
#include <seq.h>
#include <table.h>
#include <atom.h>
#include <string.h>
#include <assert.h>


Seq_T readFile(FILE *fp, Table_T table);
void print_table(Table_T table);

/********** main ********
 *
 * Main function of the program. First checks for a valid input, then calls
 * other functions to run processing depending on the type of input given.
 *
 * Parameters:
 *      int argc: number of arguments
 *      char* argv: char* of arguments
 *
 * Return: void
 *
 * Expects
 *      None
 * Notes:
 *      Will CRE if invalid files given
 ************************/

int main(int argc, char *argv[])
{ 
        Table_T table = Table_new(5, NULL, NULL); 
        Seq_T pixels = NULL;

        assert(argc == 1 || argc == 2); /* Make sure correct usage */
        
        if(argc == 1){ /* Read from standard input */
                pixels = readFile(stdin, table);
        }
        else{ /* Read from a file */
                FILE *fp = fopen(argv[1], "r");
                assert(fp != NULL);
                pixels = readFile(fp, table);
                fclose(fp);
        }
        /* set width/length */
        int width = strlen(Seq_get(pixels, 0));
        int height = Seq_length(pixels);
        
        /* Raw PGM formatting */
        printf("P5 ");
        printf("%d ", width);
        printf("%d ", height);
        printf("255 ");
        
        /* Print out image in raw format to stdoutput */
        for(int i = 0; i < height; i++){
                char* currLine = (char*)Seq_get(pixels, i);
                for(int j = 0; j < width; j ++) {
                        printf("%c", currLine[j]);
                }
        }
        /* Free all used memory */
        freeMemory(table);
        
        return EXIT_SUCCESS;
}


/********** handleLine ********
 *
 * Handles a line based on our logic. Call handleLine on a line, it will first
 * parse the line. Then it will check if we've seen the char* infusion sequence
 * before (the duped == 0 line). 
 * If we have NOT found a duplicate yet, it will check if we SHOULD add the
 * current charInfusion.
 * If we have found a duplicate, we check if the current charInfusion matches
 * the actual charInfusion, and then add the numbers accordingly.
 *
 * Parameters:
 *      Table_T table: table of charInfusion, Sequence of numbers
 *      char* line: current line being parsed from the input file
 *      char** real_infusion: real char* infusion sequence
 *      int* duped: bool representing whether we've seen a pair of sequences 
 *      before
 *
 * Return: void
 *
 * Expects
 *      Parameters given correctly
 * Notes:
 *      Will CRE if cannot make a new Seq
 ************************/
void handleLine(Table_T table, char* line, char** real_infusion, int* duped){        
        char* charSequence = NULL;
        char* numbers = NULL;
        parseLine(line, &charSequence, &numbers);
        if(line == NULL || charSequence == NULL || numbers == NULL){
                return;
        }
        free(line); /*line no longer needed*/
        const char* sequenceAtom = Atom_string(charSequence);
        /* above line may throw an error if empty sequence */
        if (*duped == 0) { /*if already found a pair*/
                if(Table_get(table, sequenceAtom) == NULL){ /*new table entry*/
                        Seq_T lines = Seq_new(1);
                        assert(lines != NULL);
                        Seq_addhi(lines, numbers);
                        Table_put(table, sequenceAtom, lines);
                }       
                else{ /*update old list*/
                        Seq_T lines = Table_get(table, sequenceAtom);
                        assert(lines != NULL);
                        *real_infusion = charSequence;
                        *duped = 1;
                        Seq_addhi(lines, numbers);
                }
        }
        else{ /*check if existing, then add*/
                if(strcmp(charSequence, *real_infusion) == 0){
                        Seq_T lines = Table_get(table, sequenceAtom);
                        assert(lines != NULL);
                        Seq_addhi(lines, numbers);
                }
        }
}

/********** readFile ********
 *
 * This function handles reading the entire file. Calls readaline to grab
 * each line, then handles each line in handleLine. Stores a table representing
 * <char* representing the right char infusion, Sequence representing 2d 
 * array of pixels of that char infusion>. Returns that sequence of pixels
 *
 * Parameters:
 *      FILE *fp: pointer to the file reader
 *      Table_T table: table where all pixels will be setored
 *
 * Return: 
 *      Seq_T representing the 2d array of correct pixels
 * Expects
 *      fp is correctly opened
 * Notes:
 *      Will call CREs inside helper functions
 ************************/
Seq_T readFile(FILE *fp, Table_T table){
        char *line = NULL;
        char *real_infusion = NULL;
        int duplicateFound = 0;
        while (readaline(fp, &line) != 0) {
                handleLine(table, line, &real_infusion, &duplicateFound);
        }
        const char* atomInfusion = Atom_string(real_infusion);
        /* above line may throw an error if empty line */
        return Table_get(table, atomInfusion);
}


/*Functions used in testing. Purely for testing. Ignore*/

/* function used for printing*/
void print_table_entry(const void *key, void **value, void *cl) {
        char *key_str = (char *)key;
        Seq_T seq = *(Seq_T *)value;
        (void)cl;
        printf("Key: %s\n", key_str);

        /*function used to print seq*/
        int seq_length = Seq_length(seq);
        for (int i = 0; i < seq_length; i++) {
                char *element = (char *)Seq_get(seq, i);
                printf("  Value[%d]: %s\n", i, element);
        }
}
/*prints table*/ 
void print_table(Table_T table) {
        Table_map(table, print_table_entry, NULL);
}
