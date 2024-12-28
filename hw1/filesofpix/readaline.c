/**************************************************************
 *
 *                     readaline.c
 *
 *     Assignment: filesofpix
 *     Authors:  lyuan04, dperki01, 
 *     Date:     9/15/24
 *
 *     summary
 *
 *     ...you may provide more information here about 
 *        the program or file, it's interfaces, etc. here
 *
 **************************************************************/

#include <readaline.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h> /* to throw CREs*/

/********** readaline ********
 *
 * Return the size of the read line, and sets the inputed char* to the first
 * char in the char ** array created from the read in line
 * Designed like the List class we had to construct in CS15, with reallocating
 * and copying, etc.
 *
 * Parameters:
 *      FILE *inputfd: pointer to the open file
 *      char **datapp: points to the string of
 *
 * Return: 
 *      size_t of the char ** array, including the '\n' and '\0' characters
 * Expects
 *      inputfd and datapp must not be NULL
 * Notes:
 *      Will CRE if inputfd or datapp is NULL
 *      May CRE if calloc fails.
 ************************/
size_t readaline(FILE *inputfd, char **datapp) {
        /* Ensure inputs are not NULL*/
        assert(inputfd != NULL && datapp != NULL); 

        /* Initialize variables */
        int curr_value;
        size_t count = 0;
        size_t n = 100;
        char *line = (char *)calloc(n, sizeof(char));/* grab a line of size n*/
        assert(line != NULL);

        /*Read char by char through the file*/
        while ((curr_value = fgetc(inputfd)) != EOF) {
                if (curr_value == '\n') {
                        break;
                }
                /*Expand buffer if char* overflows */
                if (count >= n - 1) {
                        n *= 2;
                        char *temp = (char  *)realloc(line, n * sizeof(char));
                        assert(temp != NULL);
                        line = temp;
                }
                line[count++] = (char)curr_value;
        }
        *datapp = line; /* add null terminator and repoint datapp */
        
        if (count == 0) { /*If at end of file, delete alloced memory*/
                free(line);
                *datapp = NULL;
                return 0;
        }
        assert(ferror(inputfd) == 0); /* ensure no file erorrs*/
        return count;
}