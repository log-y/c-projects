/**************************************************************
 *
 *                     freeMemory.c
 *
 *     Assignment: filesofpix
 *     Authors:  lyuan04, dperki01, 
 *     Date:     9/15/24
 *
 *     Frees all of the memory in the table we use
 *
 **************************************************************/


#include <seq.h>
#include <table.h>
#include <stdlib.h>

/********** free_table_entry ********
 *
 * Frees the entries within the table, including the Hanson sequence we used
 * to store true lines, and all lines in the sequence
 *
 * Parameters:
 *      const void *key: key of table (representing an alloced char infusion)
 *      void **value: value of table (representing a seq)
 *      void *cl: optional tracker variable
 *
 * Return: 
 *      No return
 * Expects
 *      No expectations
 * Notes:
 *      Cannot actually free the key because the key is actually an atom
 ************************/
void free_table_entry(const void *key, void **value, void *cl) {
        char *key_str = (char *)key;
        /*****
        key_str is the SINGLE, ONLY source of memory leaks in this entire 
        program. There is NO WAY for me to free the char* BEHIND an atom after
        we create it. There is also NO WAY for me to free the char* BEFORE we 
        make the atom because the atom would NOT RECOGNIZE the char*
        This is a DOCUMENTED ERROR AND ALLOWED BY THE SPEC 
        (we think, hopefully) 
        ******/
        (void)(key_str); 

        Seq_T seq = *(Seq_T *)value;
                
        /* Free each element in the Seq_T */ 
        int seq_length = Seq_length(seq);
        for (int i = 0; i < seq_length; i++) {
                char *element = (char *)Seq_get(seq, i);
                free(element); /*frees each group of numbers */
                (void)element;
        }
        /* Free Seq_T*/
        Seq_free(&seq);
        
        (void)cl;
}

/********** freeMemory ********
 *
 * Returns nothing, frees the memory used by hanson's table.
 * Frees memory of each list in Hanson's table 
 *
 * Parameters:
 *      Table_T table: a pointer to the table used throughout this program
 *
 * Return: No return
 *
 * Expects
 *      no expectations
 * Notes:
 *      no notes
 ************************/
void freeMemory(Table_T table){
        /* Free entries to table */
        Table_map(table, free_table_entry, NULL);
        /* Free table itself */
        Table_free(&table);
}