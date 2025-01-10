/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *                      instructions.c
 *      Charlotte Yamamoto (cyamam01) and Logan Yuan (lyuan04)
 *      11/15/2024
 *      Implementation for instructions module. Will call functions 
 *      built in memory and registers for execution
 * 
 *      HW6: um
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include "instructions.h"
#include "memory.h"
#include "registers.h"

/********** opcode_from_word ********
 *
 * Given a bitpacked word, takes the opcode from it
 *
 * Parameters:
 *      uint64_t word: the codeword that we will extract the opcode from
 * Return: 
 *      Um_opcode: opcode representing the codeword's opcode
 * Expects
 *      Properly parsed word
 * Notes:
 *      Will throw error if invalid opcode
 ************************/
Um_opcode opcode_from_word(uint64_t word)
{
        uint64_t opcode_maybe = Bitpack_getu(word, 4, 28);
        assert(opcode_maybe <= LV);
        return (Um_opcode) (opcode_maybe);
}

/* stores a large value for us (for comparison) */
static const uint64_t n2_32 = ((uint64_t) 2) << 32;

/********** execute ********
 *
 * Executes a given instruction
 *
 * Parameters:
 *      uint32_t word: the codeword to be executed
 *      Register_T regs: struct containing our registers
 *      Memory_T mem: struct containing our memory data structures
 *      uint32_t* counter: program counter
 * Return: 
 *      1 or 0 depending on whether halt was found
 * Expects
 *      Valid registers, counter, and memory (not null)
 *      properly parsed word
 * Notes:
 *      Will throw error for a variety of reasons. Notably, can't find enough
 *      memory, see helper functions
 *      Segfaults occasionally, we don't know why.
 ************************/
int execute(uint32_t word, Registers_T regs, Memory_T mem,  uint32_t* counter) 
{
        assert(regs != NULL && mem != NULL);
        uint64_t word64 = word;
        uint32_t a, b, c, segment_idx, word_idx;
        uint64_t value;
        uint32_t *word_p;
        Um_opcode opcode = opcode_from_word(word64);
        switch (opcode) {
                case CMOV: /* conditional move */
                        a = reg_word(word64, A);
                        b = reg_word(word64, B);
                        c = reg_word(word64, C);
                        if (get_register(regs, c) != 0) {
                                set_register(regs, a, get_register(regs, b));
                        }
                        break;
                case SLOAD: /* segment load */
                        a = reg_word(word64, A);
                        b = reg_word(word64, B);
                        c = reg_word(word64, C);

                        segment_idx = get_register(regs, b);
                        word_idx = get_register(regs, c);
                        word_p = get_segment(mem, segment_idx); 
                        set_register(regs, a, word_p[word_idx]);
                        break;
                case SSTORE: /* segmented store*/
                        a = reg_word(word64, A);
                        b = reg_word(word64, B);
                        c = reg_word(word64, C);
                        segment_idx = get_register(regs, a);
                        word_idx = get_register(regs, b);
                        value = get_register(regs, c);
                        word_p = get_segment(mem, segment_idx);
                        word_p[word_idx] = value; 
                        break;
                case ADD: /* add */
                        a = reg_word(word64, A);
                        b = reg_word(word64, B);
                        c = reg_word(word64, C);
                        value = (uint64_t)(get_register(regs, b) + 
                                                    get_register(regs, c)) 
                                                    % n2_32;
                        set_register(regs, a, (uint32_t)value);
                        break;
                case MUL: /* mul */
                        a = reg_word(word64, A);
                        b = reg_word(word64, B);
                        c = reg_word(word64, C);
                        value = (uint64_t)(get_register(regs, b) * 
                                                    get_register(regs, c)) 
                                                    % n2_32;
                        set_register(regs, a, (uint32_t)value);
                        break;
                case DIV: /* div (not by zero tho) */
                        a = reg_word(word64, A);
                        b = reg_word(word64, B);
                        c = reg_word(word64, C);
                        assert(get_register(regs, c) != 0);
                        value = (uint64_t)(get_register(regs, b) / 
                                                    get_register(regs, c));
                        set_register(regs, a, (uint32_t)value);
                        break;
                case NAND: /* not and */
                        a = reg_word(word64, A);
                        b = reg_word(word64, B);
                        c = reg_word(word64, C);
                        value = ~(get_register(regs, b) & 
                                           get_register(regs, c));
                        set_register(regs, a, (uint32_t)value);
                        break;
                case HALT: /* halt (stop) */
                        return 1;
                case ACTIVATE: /* maps a new piece of memory */
                        b = reg_word(word64, B);
                        c = reg_word(word64, C);
                        value = get_register(regs, c);
                        set_register(regs, b, map_segment(mem,(uint32_t)value));
                        break;
                case INACTIVATE: /* unmaps memory (reuses id)*/
                        c = reg_word(word64, C);
                        value = get_register(regs, c);
                        assert(value != 0);
                        unmap_segment(mem, value);
                        break;
                case OUT: /* output to stdout */
                        c = reg_word(word64, C);
                        value = get_register(regs, c);
                        assert(value <= 255);
                        putc((char)value, stdout);
                        break;
                case IN:  /* input from stdin */
                        c = reg_word(word64, C);
                        value = getc(stdin);
                        assert(value <= 255);
                        set_register(regs, c, value);
                        break;
                case LOADP: /* loads program into segment 0 */
                        b = reg_word(word64, B);
                        c = reg_word(word64, C);
                        
                        value = get_register(regs, b);
                        (*counter) = get_register(regs, c);
                        (*counter)--;
                        if(value == 0){
                                break;
                        }
                        word_p = get_segment(mem, value);
                        assert(word_p != NULL);

                        void* prev_zero = get_segment(mem, 0);
                        assert(prev_zero != NULL);
                        free(prev_zero);
                        prev_zero = NULL;

                        set_segment(mem, 0, word_p);
                        break;
                case LV: /* loads value into specified register */
                        a = Bitpack_getu(word64, 3, 25);
                        value = Bitpack_getu(word64, 25, 0);
                        set_register(regs, a, value);
                        break;
                default:
                        exit(1); 
                        break;
        }
        return 0; /* halt not found */
}

/********** reg_word ********
 *
 * parses a word of the usual format in the spec (with 3 registers)
 *
 * Parameters:
 *      uint64_t word: the codeword to be parsed
 *      RegisterNames reg_name: name of register to extract
 * Return: 
 *      uint32_t representing which register  
 * Expects
 *      reg_name to be one of the 8 valid registers
 * Notes:
 *      None
 ************************/
uint32_t reg_word(uint64_t word, RegisterNames reg_name)
{
        uint32_t reg = Bitpack_getu(word, 3, reg_name * 3);
        assert(reg <= 7);
        return reg;
}

