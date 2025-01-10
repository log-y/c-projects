/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *                      memory.h
 *      Charlotte Yamamoto (cyamam01) and Logan Yuan (lyuan04)
 *      11/15/2024
 *      interface for Memory module
 *      HW6: um
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef Memory_H
#define Memory_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define T Memory_T
typedef struct T *T;

T memory_new();
uint32_t map_segment(T memory, int size);
void initialize_segment_zero(T memory, uint32_t* program);
void unmap_segment(T memory, uint32_t id);
void* get_segment(T memory, uint32_t id);
void free_memory(T* memory);
void print_seq(T memory);
void set_segment(T memory, uint32_t id, void* ptr);

#undef T
#endif