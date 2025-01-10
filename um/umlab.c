/*
 * umlab.c
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *  
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.  
 * 
 * Any additional functions and unit tests written for the lab go
 * here. 
 *  
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc);
Um_instruction loadval(unsigned ra, unsigned val);


/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}


static inline Um_instruction mul(Um_register a, Um_register b, Um_register c) 
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction div(Um_register a, Um_register b, Um_register c) 
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction cmov(Um_register a, Um_register b, Um_register c) 
{
        return three_register(CMOV, a, b, c);
}

static inline Um_instruction in(Um_register c) 
{
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction out(Um_register c) 
{
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction sload(Um_register a, Um_register b, Um_register c) 
{
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction sstore(Um_register a, Um_register b, Um_register c) 
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c) 
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction map(Um_register b, Um_register c) 
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmap(Um_register c) 
{
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction loadp(Um_register b, Um_register c) 
{
        return three_register(LOADP, 0, b, c);
}


/* Functions for working with streams */

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }
      
}


/* Unit tests for the UM */

void build_halt_test(Seq_T stream)
{
        append(stream, halt());
}

void build_verbose_halt_test(Seq_T stream)
{
        append(stream, halt());
        append(stream, loadval(r1, 'B'));
        append(stream, out(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, out(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, out(r1));
        append(stream, loadval(r1, '!'));
        append(stream, out(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, out(r1));
}


Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        uint64_t word = 0;
        word = Bitpack_newu(word, 4, 28, (uint64_t)op);
        word = Bitpack_newu(word, 3, 6, (uint64_t)ra);
        word = Bitpack_newu(word, 3, 3, (uint64_t)rb);
        word = Bitpack_newu(word, 3, 0, (uint64_t)rc);

        return (Um_instruction)(word);
}

Um_instruction loadval(unsigned ra, unsigned val)
{
        uint64_t word = 0;
        word = Bitpack_newu(word, 4, 28, (uint64_t)LV);
        word = Bitpack_newu(word, 3, 25, (uint64_t)ra);
        word = Bitpack_newu(word, 25, 0, (uint64_t)val);
        return (Um_instruction)(word);
}

void build_add_test(Seq_T stream)
{
        append(stream, add(r1, r2, r3));
        append(stream, halt());
}

void build_output_test(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r1, r2));
        append(stream, out(r3));
        append(stream, halt());
}

void build_cmovt_test(Seq_T stream)
{
        append(stream, loadval(r4, 1));
        append(stream, loadval(r5, 48));
        append(stream, loadval(r6, 5));
        append(stream, cmov(r6, r5, r4));
        append(stream, out(r6));
        append(stream, halt());
}

void build_cmovf_test(Seq_T stream)
{
        append(stream, loadval(r4, 0));
        append(stream, loadval(r5, 48));
        append(stream, loadval(r6, 5));
        append(stream, cmov(r6, r5, r4));
        append(stream, add(r0, r5, r6));
        append(stream, out(r0));
        append(stream, halt());
}

void build_mul_test(Seq_T stream)
{
        append(stream, loadval(r2, 8));
        append(stream, loadval(r3, 10));
        append(stream, mul(r1, r2, r3));
        append(stream, out(r1));
        append(stream, halt());
}

void build_segments_test(Seq_T stream)
{
        append(stream, loadval(r3, 8));
        append(stream, loadval(r2, 2)); /* index in segment */
        append(stream, loadval(r1, 0)); /* which segment */
        append(stream, loadval(r4, 1)); /* which segment */
        append(stream, loadval(r5, 77));
        append(stream, map(r2, r3)); /*# of words in r3, id in r2. 8 words r2*/
        
        append(stream, sstore(r4, r1, r5)); /* [1][0] = 999 */

        append(stream, sload(r7, r4, r1));

        append(stream, out(r7));
        append(stream, halt());
}

void build_segments_two_test(Seq_T stream)
{
        append(stream, loadval(r3, 8));
        append(stream, loadval(r2, 2)); /* index in segment */
        append(stream, loadval(r1, 0)); /* which segment */
        append(stream, loadval(r4, 1)); /* which segment */
        append(stream, loadval(r5, 77));
        append(stream, loadval(r0, 2));
        append(stream, map(r2, r3)); /* loads 8 words, puts id in 2 */

        append(stream, sstore(r4, r2, r6));
        
        append(stream, sstore(r4, r1, r5)); /* [1][0] = 244 */

        append(stream, sload(r7, r4, r1));

        append(stream, out(r7));
        append(stream, halt());
}

void build_activate_inactivate_load(Seq_T stream)
{
        append(stream, loadval(r0, 5));
        append(stream, loadval(r1, 30));
        append(stream, loadval(r2, 2));
        append(stream, loadval(r3, 78));
        append(stream, map(r0, r1));

        append(stream, sstore(r0, r2, r3));
        append(stream, sload(r4, r0, r2));  /* r4 = m[5][2] */

        append(stream, unmap(r0));
        append(stream, out(r4));
        append(stream, halt());
}

void build_loadp_test(Seq_T stream)
{
        append(stream, loadval(r0, 5));
        append(stream, loadval(r1, 5));
        append(stream, loadval(r2, 0));
        append(stream, loadval(r3, 78));
        append(stream, loadval(r5, 0));
        append(stream, loadval(r6, 6));
        append(stream, loadval(r7, 7));

        append(stream, map(r0, r1)); /* new segment in register 5 */

        append(stream, sstore(r0, r5, r3)); /* 0 is 78 */

        append(stream, loadval(r5, 1));
        append(stream, loadval(r3, 79));
        append(stream, sstore(r0, r5, r3)); /* 1 is 79 */

        append(stream, loadval(r5, 2));
        append(stream, loadval(r3, 80));
        append(stream, sstore(r0, r5, r3)); /* 2 is 80 */

        append(stream, loadval(r5, 3));
        append(stream, loadval(r3, 81));
        append(stream, sstore(r0, r5, r3)); /* 3 is 81 */

        append(stream, loadval(r5, 4));
        append(stream, loadval(r3, 82));
        append(stream, sstore(r0, r5, r3)); /* 4 is 82 */

        append(stream, map(r6, r3));
        append(stream, map(r7, r3));

        // append(stream, unmap(r7));
        append(stream, loadval(r5, 2));

        append(stream, loadp(r5, r5));

        append(stream, halt());
}

