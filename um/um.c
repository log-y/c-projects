/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *                      main.c
 *      Charlotte Yamamoto (cyamam01) and Logan Yuan (lyuan04)
 *      11/15/2024
 *      The main function that will run our .um project. This will read in the 
 *      .um file and run it 
 * 
 *      HW6: um
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "assert.h"

#include "registers.h"
#include "instructions.h"
#include "memory.h"

/* const variables will store info about codewords */
const unsigned BITS_CHAR = 8;
const unsigned CHARS_WORD = 32 / BITS_CHAR;

uint32_t *load_initial_segment(char *filename);

void print_all_regs(Registers_T regs);

/********** main ********
 *
 * Loads in um file and executes each instruction
 *
 * Parameters:
 *      int argc: number of inputs
 *      int argv[]: array of inputs
 * Return: 
 *      int: error code
 * Expects
 *      valid format provided
 * Notes:
 *      Will throw an error can't find enough memory at any time in program
 *      Will throw an error for any failure specified in the spec
 ************************/
int main(int argc, char *argv[])
{
        if (argc != 2)
        {
                fprintf(stderr, "usage: ./um [ program-name.um ]\n");
                return EXIT_FAILURE;
        }

        char *filename = argv[1];

        /* read file and store in program*/
        uint32_t *program = load_initial_segment(filename);

        Registers_T regs = registers_new();
        Memory_T my_memory = memory_new();

        /* add program to segment zero */
        initialize_segment_zero(my_memory, program); 

        int halt = 0;
        uint32_t counter = 0;

        /* loops thru until halt instruction */
        while (halt != 1)
        {
                uint32_t word = ((uint32_t*)get_segment(my_memory, 0))[counter];
                halt = execute(word, regs, my_memory, &counter);
                counter++;
        }

        free_memory(&my_memory);
        free_registers(&regs);
        return EXIT_SUCCESS;
}


/********** load_intial_segment_old ********
 *
 * Loads the .um file into a single array in memory and returns the pointer
 *
 * Parameters:
 *      char* filename: name of input file
 * Return: 
 *      uint32_t* representing pointer to array of codewords
 * Expects
 *      Valid filename
 * Notes:
 *      Will throw an error can't find enough memory
 ************************/
uint32_t *load_initial_segment(char *filename)
{
        struct stat file_info;

        assert(stat(filename, &file_info) != -1);

        char *program = malloc(file_info.st_size);
        assert(program != NULL);

        FILE *file = fopen(filename, "rb");
        assert(file != NULL);

        size_t bytes_read = fread(program, 1, file_info.st_size, file);

        assert(bytes_read == (size_t)file_info.st_size);
        fclose(file);
        uint32_t *conv = malloc(bytes_read);
        assert(conv != NULL);
        uint64_t temp = 0;

        for (int i = 0; i < (int)bytes_read; i++)
        {
                // fprintf(stderr, "%hu\n", *(program + i)); //bits_char 8, chars_word 4
                temp = Bitpack_newu(temp, BITS_CHAR,
                                    BITS_CHAR * (CHARS_WORD - (i % CHARS_WORD) - 1),
                                    (uint64_t)(*(program + i) & 0xff));
                if (i % CHARS_WORD == CHARS_WORD - 1)
                {
                        conv[(i / CHARS_WORD)] = (uint32_t)temp;
                        temp = 0;
                }
        }
        free(program);
        return conv;
}

/********** print_all_regs ********
 *
 * Prints contents of all 8 registers in regs
 *
 * Parameters:
 *      Registers_T regs: struct containing all of our registers
 * Return: 
 *      void
 * Expects
 *      Valid Registers_T object
 * Notes:
 *      Prints to stderr
 ************************/
void print_all_regs(Registers_T regs)
{
        fprintf(stderr,
                "r0: %u\tr1: %u\tr2: %u\tr3: %u\t"
                "r4: %u\tr5: %u\tr6: %u\tr7: %u\n",
                get_register(regs, 0), get_register(regs, 1),
                get_register(regs, 2), get_register(regs, 3),
                get_register(regs, 4), get_register(regs, 5),
                get_register(regs, 6), get_register(regs, 7));
}
