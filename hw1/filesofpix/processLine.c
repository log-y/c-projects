/**************************************************************
 *
 *                     processLine.c
 *
 *     Assignment: filesofpix
 *     Authors:  lyuan04, dperki01, 
 *     Date:     9/15/24
 *
 *      Handles the processing of each line: parsing each line, deciding 
 *      how to add the line to the table.
 *      
 * 
 *      Our central data structure is a Hanson table, which we use to check 
 *      for matching character infusion sequences. We also use a Hanson 
 *      sequence to hold all of the pixel rows for the corrected p5 image.
 *     
 *
 **************************************************************/


#include <table.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/********** isDigit ********
 *
 * Checks if a char is an ascii digit
 *
 * Parameters:
 *      char: a character
 *
 * Return: 1 if char is ascii digit, 0 if otherwise
 *
 * Expects
 *      None
 * Notes:
 *      None
 ************************/
int isDigit(char c){
        return c <= 57 && c >= 48;
}

/********** countBytesNeeded ********
 *
 * Given a line from the file, it counts the number of bytes needed to store 
 * the character infusion, and the number of bytes needed to store the pixel 
 * values.
 *
 * Parameters:
 *      char* s: represents the line
 *      int* numChars: pointer to string representing infusion
 *      int* numNumbers: pointer to string representing pixels
 *      int* i: iterator used
 *
 * Return: 
 *      void
 * Expects
 *      numChars and numNumbers to be alloced correctly
 * Notes:
 *      None
 ************************/
void countBytesNeeded(char* s, int* numChars, int* numNumbers, int* i){
        while(s[*i] != '\0'){
                if(isDigit(s[*i]) && isDigit(s[*i + 1]) && isDigit(s[*i + 2])){
                        (*numNumbers)++;
                        *i += 3;
                }
                else if(isDigit(s[*i]) && isDigit(s[*i + 1])){
                        (*numNumbers)++;
                        *i += 2;
                }
                else if(isDigit(s[*i])){
                        (*numNumbers)++;
                        (*i)++;
                }
                else{
                        (*numChars)++;
                        (*i)++;
                }
        }
}

/********** ci (character to integer) ********
 *
 * Given a character and a place representing its place in a number,
 * returns an integer representing its value
 * 
 *
 * Parameters:
 *      char c: ascii digit character
 *      int place: it's algebraic 'place' (tens place, hundreds place, etc)
 *
 * Return:
 *      Returns an integer representing its value
 * Expects
 *      None
 * Notes:
 *      None
 ************************/
int ci(char c, int place){
        return place * (c - 48);
}

/********** fillInStrings ********
 *
 * Fills in strings with their appropriate values. Nums* represents an
 * array of pixels so nums will be filled with ascii values representing
 * pixel values. Chars* represents a char infusion so char will be turned
 * into a char infusion string.
 *
 * Parameters:
 *      char* s: represents the line we are parsing
 *      int* i: iterator we're using
 *      char* nums: represents string of pixels
 *      char* chars: represents string of char infusion
 *
 * Return:
 *      No return value
 * Expects
 *      Expects that s, i, nums, and chars are not NULL
 * Notes:
 *      No notes
 ************************/
void fillInStrings(char* s, int* i, char* nums, char* chars){
        int j = 0; /* iterator for numbers */
        int k = 0; /* iterator for characters */
        while(s[*i] != '\0'){
                if(isDigit(s[*i]) && isDigit(s[*i + 1]) && isDigit(s[*i + 2])){
                        int hundreds = ci(s[*i], 100);
                        int tens = ci(s[*i + 1], 10);
                        int ones = ci(s[*i + 2], 1);
                        nums[j++] = hundreds + tens + ones;
                        *i += 3;
                }
                else if(isDigit(s[*i]) && isDigit(s[*i + 1])){
                        nums[j++] = (10 * (s[*i] - 48) + (s[*i + 1] - 48));
                        *i += 2;
                }
                else if(isDigit(s[*i])){
                        nums[j++] =  (s[*i] - 48);
                        (*i)++;
                }
                else{
                        chars[k++] = s[*i];
                        (*i)++;
                }
        }
        nums[j] = '\0';
        chars[k] = '\0';
}

/********** parseLine ********
 *
 * Returns nothing, parses the inputed char* s into ascii numerals, set to the 
 * address of char ** characters and char** numbers to the char* of ASCII
 * numerals
 * 
 *
 * Parameters:
 *      char* s: represents the original char* containing the corrupted line
        char** characters: pointer to the address of the char* off all chars
 *      char** numbers: points to the char* of all ACII numerals
 *
 * Return: no return
 *
 * Expects
 *      char* s must not be NULL
 * Notes:
 *      Will CRE if s is NULL
 *      May CRE if calloc fails.
 ************************/
void parseLine(char* s, char** characters, char** numbers){
        assert(s != NULL);
        /* initialize all variables*/
        int i = 0;
        int numChars = 0;
        int numNumbers = 0;
        /* Count number of bytes containing nonnumeric chars, and number
           groups containing numeric chars (0-9), until end of line */
        while(s[i] != '\0'){
                countBytesNeeded(s, &numChars, &numNumbers, &i);
        }
        
        i = 0; /* Reset i */
        /* Create new char* to contain all numeric chars from the read line and
           all nonnumeric chars in another char* array, the size of each is the
           size of numNumbers + 1 and numChars + 1 respectively */
        char* nums = calloc(numNumbers + 1, sizeof(char));
        char* chars = calloc(numChars + 1, sizeof(char));
        assert(nums!= NULL && chars != NULL);
        /* Add values to each string */
        fillInStrings(s, &i, nums, chars);
        /* Set inputed char* to the newly parsed ones */
        *characters = chars;
        *numbers = nums;
}