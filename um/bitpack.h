/*************************************************************************
 *
 *                     bitpack.h
 *
 *     Assignment: arith
 *     Authors:  lyuan04, dperki01
 *     Date:     10/23/24
 *
 *     Provides the user with the ability access and edit the bit values
 *     within signed and unsigned 64bit integers through the declared
 *     functions.
 *
 *************************************************************************/
#ifndef BITPACK_INCLUDED
#define BITPACK_INCLUDED
#include <stdbool.h>
#include <stdint.h>
#include "assert.h"

/* shifting helper functions */
uint64_t left_shift(uint64_t value, unsigned shift);
uint64_t right_shift_u(uint64_t value, unsigned shift);
int64_t right_shift_s(int64_t value, unsigned shift);

/*  width test functions */
bool Bitpack_fitsu(uint64_t n, unsigned width);
bool Bitpack_fitss( int64_t n, unsigned width);

/* field extraction functions */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb);
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb);

/* field update functions */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value);
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                      int64_t value);

#endif