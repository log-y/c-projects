/*************************************************************************
 *
 *                     bitpack.c
 *
 *     Assignment: arith
 *     Authors:  lyuan04, dperki01
 *     Date:     10/23/24
 *
 *     Contains the implementation for bitpack functions that are 
 *     declared within bitpack.h
 *
 *************************************************************************/
#include <stdbool.h>
#include <stdint.h>
#include "assert.h"
#include <stdio.h>

#include "bitpack.h"

Except_T Bitpack_Overflow = {"Overflow packing bits"};

/********** left_shift ********
 *
 * Shifts the given number to the left by 'shift' number of bits when in 
 * binary format, essentially multiplying the value by 2^shift. 
 * A shift of 64 returns 0.
 *
 * Parameters:
 *      uint64_t value: 64bit representation of the number to be shifted
 *      unsigned shift: amount of indexes to shift the number by in binary form
 * Return: 
 *      the result of the shift as a uint64 variable
 * Expects
 *      shift <= 64
 * Notes:
 *      will CRE if shift > 64
 ************************/
uint64_t left_shift(uint64_t value, unsigned shift)
{
        assert(shift <= 64);

        if (shift == 0) {
                return value; /* no change to value */
        }

        if (shift == 64) {/* wipe the uint64_t clean*/
                return 0; /* return 0 */
        }

        uint64_t result = value << shift; /* shift left */
        return result;
}

/********** right_shift_u ********
 *
 * Specifically handles right shifting unsigned 64bit integers, shifts
 * to the right by a 'shift' number of bits when inputted value is in binary
 * format. A shift of 64 returns 0.
 *
 * Parameters:
 *      uint64_t value: 64bit representation of the number to be shifted
 *      unsigned shift: amount of indexes to shift the number by in binary form
 * Return: 
 *      the result of the shift as a uint64 variable
 * Expects
 *      shift <= 64
 * Notes:
 *      will CRE if shift > 64
 ************************/
uint64_t right_shift_u(uint64_t value, unsigned shift)
{
        assert(shift <= 64);

        if (shift == 0) {
                return value; /* no change to value */
        }

        if (shift == 64) {/* wipe the uint64_t clean */
                return 0; /* return 0 */
        }

        uint64_t result = value >> shift; /* shift right */
        return result;
}

/********** right_shift_s ********
 *
 * Specifically handles right shifting signed 64bit integers, shifts
 * to the right by a 'shift' number of bits when inputted value is in binary
 * format. A shift of 64 returns 0 for positive input values and -1 for 
 * negative input values.
 *
 * Parameters:
 *      int64_t value: 64bit representation of the number to be shifted
 *      unsigned shift: amount of indexes to shift the number by in binary form
 * Return: 
 *      the result of the shift as a int64 variable
 * Expects
 *      shift <= 64
 * Notes:
 *      will CRE if shift > 64
 ************************/
int64_t right_shift_s(int64_t value, unsigned shift)
{
        assert(shift <= 64);

        if (shift == 64){       /* return 0 if value was positive */
                return (value < 0) ? -1 : 0; /* return -1 if value was */
        }                                    /* negative */

        if (shift == 0) {
                return value; /* value remains unchanged */
        }

        uint64_t result = value >> shift; /* shift right */

        if (value > 0) { /* if value was positive, return shift as is*/
                return result;
        } else { /* if value was negative, ensure sign extension correct */
                /* create mask to extend sign bit */
                int64_t mask = ((INT64_MAX) << (64 - shift));
                /* bitwise or */
                return result | mask;
        }
}

/********** Bitpack_fitsu ********
 *
 * Returns true if the inputted uint64 value can be fit within a given
 * number of bits which represent a field of a word.
 *
 * Parameters:
 *      uint64_t n: 64bit representation of the number being checked
 *      unsigned width: number of bits the value is attempting to fit in
 * Return: 
 *      a boolean value, true if can fit, false if cannot
 * Expects
 *      width <= 64
 * Notes:
 *      will CRE if width > 64
 ************************/
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
        assert(width <= 64);

        if (width == 0) { /* cannot fit in width of zero */
                return false;
        }
        /* get maximum value possible using width bits */
        uint64_t max_val = left_shift((uint64_t)1, width); /* 2^width*/
        /* return true if n is less than the max value */
        return n < max_val;
}

/********** Bitpack_fitss ********
 *
 * Returns true if the inputted int64 value can be fit within a given
 * number of bits which represent a field of a word.
 *
 * Parameters:
 *      int64_t n: 64bit representation of the number being checked
 *      unsigned width: number of bits the value is attempting to fit in
 * Return: 
 *      a boolean value, true if can fit, false if cannot
 * Expects
 *      width <= 64
 * Notes:
 *      will CRE if width > 64
 ************************/
bool Bitpack_fitss(int64_t n, unsigned width)
{
        assert(width <= 64);

        if (width == 0) { /* cannot fit in width of zero */
                return false;
        }
        /* check within bounds of signed ints created using width 
           number of bits */
        /* -2^(width-1) */
        int64_t lower_bound = -(int64_t)(1 << (width - 1));
        /* 2^(width-1) - 1*/
        int64_t upper_bound = (int64_t)((1 << (width - 1)) - 1);

        if (width == 64) {     /* return true if n fits in 64 bit*/
                /* should always be true */
                return n >= INT64_MIN && n <= INT64_MAX;
        }
        /* returns true if n is in bounds of width */
        return n >= lower_bound && n <= upper_bound;
}

/********** Bitpack_getu ********
 * 
 * Finds a field in the inputted word by accessing the address of the
 * field's least significant variable, and returns the unsigned value stored 
 * within the width of that field
 *
 * Parameters:
 *      uint64_t word: 64bit word containing field to be accessed
 *      unsigned width: number of bits the value is attempting to fit in
        unisgned lsb: least significant variable of field
 * Return: 
 *      a boolean value, true if can fit, false if cannot
 * Expects
 *      width <= 64
 * Notes:
 *      will CRE if width > 64
 ************************/
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

        if (width == 0) {
                return 0; /* fields of width 0 contain the value 0 */
        }

        /* create a mask of width bits, (e.g., if width = 4, mask = 0b1111) */
        uint64_t mask = left_shift(1, width) - 1;

        /* right shift word by lsb and apply mask */
        return right_shift_u(word, lsb) & mask;
}

/********** Bitpack_gets ********
 * 
 * Finds a field in the inputted word by accessing the address of the
 * field's least significant variable, and returns the signed value stored 
 * within the width of that field
 *
 * Parameters:
 *      uint64_t word: 64bit word containing field to be accessed
 *      unsigned width: number of bits the value is attempting to fit in
        unisgned lsb: least significant variable of field
 * Return: 
 *      a boolean value, true if can fit, false if cannot
 * Expects
 *      width <= 64
 * Notes:
 *      will CRE if width > 64
 ************************/
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

        if (width == 0) {
                return 0; /* fields of width 0 contain the value 0 */
        }

        /* create a mask of width bits, (e.g., if width = 4, mask = 0b1111) */
        /* casting 1 to uint64_t */
        uint64_t mask = left_shift((uint64_t)1, width) - 1; 

        /* right shift word by lsb and apply mask */
        uint64_t value = right_shift_u(word, lsb) & mask;

        /* check that sign bit is set */
        if (value & left_shift((uint64_t)1, width - 1))
        { /* casting 1 to uint64_t */
                /* if sign bit is set, then value is negative */
                /* subtract 1 << width from the masked value */
                value = value - left_shift((uint64_t)1, width);
        }

        return (int64_t)value;
}

/********** Bitpack_newu ********
 *
 * Places an unsigned value into a uint64 at a specificied location and width
 *
 * Parameters:
 *      uint64_t n: 64bit representation of the number being written to
 *      unsigned width: number of bits the value is attempting to fit in
 *      unsigned lsb: the location of the least significant bit it will be 
 *                      written to
 *      uint64_t value: the unsigned value that is going to be written
 * Return: 
 *      a uint64_t representing 'word' with a value fitted into the specified
 *      spot
 * Expects
 *      width <= 64, width+lsb<=64
 * Notes:
 *      will CRE if expectations are not met
 ************************/
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, 
                      uint64_t value)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

        /* check if the value fits in the specified width */
        if (!Bitpack_fitsu(value, width)) {/* raise exception if doesn't fit */
                RAISE(Bitpack_Overflow);
        }

        /* create mask to fill field in word with */
        uint64_t mask = left_shift((uint64_t)1, width) - 1;
        mask = left_shift(mask, lsb); /* shift mask to correct position */
        mask = ~mask;                 /* invert the mask to clear it */

        word = word & mask;                  /* clear field in original word */
        word = word | left_shift(value, lsb); /* insert new value */

        return word;
}

/********** Bitpack_news ********
 *
 * Places a signed value into a uint64 at a specificied location and width
 *
 * Parameters:
 *      uint64_t word: 64bit word representation of the number being written to
 *      unsigned width: number of bits the value is attempting to fit in
 *      unsigned lsb: the location of the least significant bit it will be 
 *                      written to
 *      int64_t value: the value that is going to be written
 * Return: 
 *      a uint64_t representing 'word' with a value fitted into the specified
 *      spot
 * Expects
 *      width <= 64, width+lsb<=64
 * Notes:
 *      will CRE if expectations are not met
 ************************/
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb, 
                      int64_t value)
{
        assert(width <= 64);
        assert(width + lsb <= 64);

        /* check if value fits in specified width */
        if (!Bitpack_fitss(value, width)) {
                RAISE(Bitpack_Overflow);
        }

        /* create mask to fill field in word with */
        uint64_t mask = left_shift((uint64_t)1, width) - 1;
        mask = left_shift(mask, lsb); /* shift mask to correct position */
        mask = ~mask;                 /* invert the mask to clear it */

        /* clear field in original word */
        word = word & mask;
        /* ensure sign value is correctly masked and inserted */
        uint64_t sign_value = (uint64_t)(value & (left_shift((int64_t)1, 
                                                             width) - 1));
        /* insert mask value at correct position */
        word = word | left_shift(sign_value, lsb); 

        return word;
}