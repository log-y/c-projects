/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *                      registers.h
 *      Charlotte Yamamoto (cyamam01) and Logan Yuan (lyuan04)
 *      11/15/2024
 *      interface for registers module
 *      HW6: um
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Registers_H
#define Registers_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define T Registers_T
typedef struct T *T;

T registers_new();
uint32_t get_register(T registers, uint32_t reg);
void set_register(T registers, uint32_t reg, uint32_t val);
void free_registers(T *regs);

#undef T
#endif