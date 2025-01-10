/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *                      registers.c
 *      Charlotte Yamamoto (cyamam01) and Logan Yuan (lyuan04)
 *      11/15/2024
 *      Implementation for registers module. Uses an underlying array
 *      to keep track of our registers.
 * 
 *      HW6: um
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "registers.h"

#define T Registers_T

/* array that represents our set of registers */
struct Registers_T{
        uint32_t registers[8];
};

/********** registers_new ********
 *
 * Creates a new registers_t struct and initializes 
 * underlying array to 0
 *
 * Parameters:
 *      None
 * Return: 
 *      A pointer to a Register_t struct
 * Expects
 *      None
 * Notes:
 *      Will throw an error if unable to find memory
 *      User must free registers class
 ************************/
T registers_new(){
        T registers = malloc(sizeof(*registers));
        assert(registers != NULL);
        for(int i = 0; i < 8; i++){
                registers->registers[i] = 0;
        }
        return registers;
}

/********** get_registers********
 *
 * returns the element at a given register
 *
 * Parameters:
 *      T registers: the set of registers to query
 *      uint32_t reg: index of register in set
 * Return: 
 *      A uint32_t representing the item at the specified
 *      index
 * Expects
 *      None
 * Notes:
 *      Will throw an error if reg out of bounds
 *      Will throw an error if invalid registers
 ************************/
uint32_t get_register(T registers, uint32_t reg){
        assert(registers != NULL);
        assert(reg <= 7);
        return registers->registers[reg];
}

/********** set_register********
 *
 * Sets an element at a certain index to a specified value
 *
 * Parameters:
 *      T registers: the set of registers to query
 *      uint32_t reg: index of register in set
 *      uin32_t val: value to replace at index
 * Return: 
 *      None
 * Expects
 *      None
 * Notes:
 *      Will throw an error if reg out of bounds
 *      Will throw an error if invalid registers
 ************************/
void set_register(T registers, uint32_t reg, uint32_t val){
        assert(registers != NULL);
        assert(reg <= 7);
        registers->registers[reg] = val;
}

/********** free_registers********
 *
 * Frees registers in array
 * 
 * Parameters:
 *      T* regs: a pointer to the registers struct to be freed
 * Return: 
 *      None
 * Expects
 *      None
 * Notes:
 *      Will throw an error if reg is an invalid object
 ************************/
void free_registers(T *regs){
        assert(regs != NULL);
        assert((*regs) != NULL);
        free(*regs);
}

#undef T