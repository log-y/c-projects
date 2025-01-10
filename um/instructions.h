/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *                      instructions.h
 *      Charlotte Yamamoto (cyamam01) and Logan Yuan (lyuan04)
 *      11/15/2024
 *      interface for instructions module
 *      HW6: um
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "stdlib.h"
#include "stdio.h"
#include <inttypes.h>
#include "stdbool.h"
#include "bitpack.h"
#include "assert.h"
#include "registers.h"
#include "memory.h"

/* stores the possible opcodes */
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

/* locations of each register from the right */
typedef enum regs { A = 2, B = 1, C = 0 } RegisterNames;

int execute(uint32_t word, Registers_T regs, Memory_T mem, uint32_t* counter);
uint32_t reg_word(uint64_t word, RegisterNames reg_name);
Um_opcode opcode_from_word(uint64_t word);

