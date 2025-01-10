/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *                      memory.c
 *      Charlotte Yamamoto (cyamam01) and Logan Yuan (lyuan04)
 *      11/15/2024
 *      Implementation for Memory module
 *      HW6: um
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <uarray.h>
#include <seq.h>


#include "memory.h"

#define T Memory_T

/* data structures that represent how we store our segments */
struct Memory_T{
        Seq_T mapped;
        Seq_T reusable; /* stores indexes in mapped of NULL pointers */
};

/********** memory_new ********
 *
 * Creates a new memory_t struct and initializes 
 *
 * Parameters:
 *      None
 * Return: 
 *      A pointer to a Memory_t struct
 * Expects
 *      None
 * Notes:
 *      Will throw an error if unable to find memory
 *      User needs to free the memory allocated 
 ************************/
T memory_new(){
        Seq_T mapped = Seq_new(5);
        assert(mapped != NULL);
        Seq_T reusable = Seq_new(5);
        assert(reusable != NULL);

        Memory_T memory = malloc(sizeof(*memory));
        assert(memory != NULL);

        memory->mapped = mapped;
        memory->reusable = reusable;

        return memory;
}

/********** map_segment ********
 *
 * Maps a new segment and returns its 32-bit identifier
 *
 * Parameters:
 *      T memory: memory struct representing how we track our IDs
 *      int size: number of codewords we need to map
 * Return: 
 *      32-bit ID representing its location in the mapped array
 * Expects
 *      None
 * Notes:
 *      Will throw an error if unable to find memory
 *      Will throw an error if given any null pointers
 *      User should free segment (either by free'ing whole struct, or unmap)
 ************************/
uint32_t map_segment(T memory, int size){
        assert(memory != NULL);
        Seq_T mapped = memory->mapped;
        assert(mapped != NULL);

        /* assert if we've used the maximum amount of keys */
        assert((uint32_t)Seq_length(mapped) < (uint32_t)0xFFFFFFFF);

        Seq_T reusable = memory->reusable;
        assert(reusable != NULL);

        uint32_t* words = calloc(size, sizeof(uint32_t));
        assert(words != NULL);

        /* checks where to find the new ID */
        if(Seq_length(reusable) == 0){
                Seq_addhi(mapped, words);
                return (uint32_t)(Seq_length(mapped) - 1);
        }
        else{
                uint32_t free_index = (uint32_t)(uintptr_t)Seq_remhi(reusable);
                Seq_put(mapped, (int)free_index, words);
                return free_index;
        }  
}

/********** initialize_segment_zero ********
 *
 * Initializes the zero segment in beginning of our program
 * Designed to only be used ONCE (at the very beginning)
 *
 * Parameters:
 *      T memory: memory struct representing how we track our IDs
 *      uint32_t* program: the fully loaded .um file as a uint32_t array
 * Return: 
 *      Void
 * Expects
 *      None
 * Notes:
 *      Will throw an error if given any null pointers
 ************************/
void initialize_segment_zero(T memory, uint32_t* program){
        assert(memory != NULL);
        Seq_T mapped = memory->mapped;
        assert(mapped != NULL);

        Seq_addhi(mapped, program);
}

/********** unmap_segment ********
 *
 * Unmaps a segment
 *
 * Parameters:
 *      T memory: memory struct representing how we track our IDs
 *      int id: Id of segment that needs to be freed
 * Return: 
 *      
 * Expects
 *      Valid inputs
 * Notes:
 *      Will throw an error if unable to find memory
 *      Will throw an error if given any null pointers
 *      Will throw an error if given an invalid ID (already unmapped)
 ************************/
void unmap_segment(T memory, uint32_t id){
        assert(memory != NULL);
        Seq_T mapped = memory->mapped;
        Seq_T reusable = memory->reusable;
        assert(mapped != NULL && reusable != NULL);

        assert((int)id < Seq_length(mapped));

        uint32_t* words = Seq_get(mapped, (int)id);
        assert(words != NULL);

        free(words);
        Seq_put(mapped, (int)id, NULL);

        Seq_addhi(reusable, (void *)(uintptr_t)id);
}

/********** get_segment ********
 *
 * Returns the codeword array behind any segment ID
 *
 * Parameters:
 *      T memory: memory struct representing how we track our IDs
 *      int id: Id of segment that needs to be freed
 * Return: 
 *      void pointer
 * Expects
 *      Valid ID
 * Notes:
 *      Will throw an error if invalid ID
 *      Will throw an error if any NULL pointers are given in the struct
 ************************/
void* get_segment(T memory, uint32_t id){
        assert(memory != NULL);
        Seq_T mapped = memory->mapped;
        assert(mapped != NULL);

        void* element = Seq_get(mapped, id);
        assert(element != NULL);

        return element;
}

/********** set_segment ********
 *
 * sets a specified segment to another uint32_t array
 *
 * Parameters:
 *      T memory: memory struct representing how we track our IDs
 *      int id: Id of segment that needs to be freed
 *      void*: codeword array to be assigned
 * Return: 
 *      Void
 * Expects
 *      Valid ID
 * Notes:
 *      Will throw an error if invalid ID
 *      Will throw an error if any NULL pointers are given in the struct
 ************************/
void set_segment(T memory, uint32_t id, void* ptr){
        assert(memory != NULL);
        Seq_T mapped = memory->mapped;
        assert(mapped != NULL);

        Seq_put(mapped, id, ptr);
}

/********** free_memory ********
 *
 * Frees all the memory used for memory
 *
 * Parameters:
 *      T *memory: pointer to our memory module
 * Return: 
 *      Void
 * Expects
 *      Valid memory pointer
 * Notes:
 *      Will throw an error if any NULL pointers are given in the struct
 ************************/
void free_memory(T* memory){
        assert(memory != NULL);
        Seq_T mapped = (*memory)->mapped;
        Seq_T reusable = (*memory)->reusable;

        assert(mapped != NULL && reusable != NULL);
        /* loops through sequence and frees each non-null element */
        for(int i = Seq_length(mapped) - 1; i >= 0; i--){
                void* used_segment = Seq_get(mapped, i);
                if(used_segment == NULL){
                        continue;
                }
                else{
                        free(used_segment);
                }
        }

        Seq_free(&mapped);
        Seq_free(&reusable);
        assert(*memory != NULL);
        free(*memory);
}


/********** print_seq********
 *
 * Prints all item in the sequence of mapped IDs
 *
 * Parameters:
 *      T memory: pointer to our memory module
 * Return: 
 *      Void
 * Expects
 *      Valid memory pointer
 * Notes:
 *      Will throw an error if any NULL pointers are given in the struct
 ************************/
void print_seq(T memory){
        printf("used indexes: ");
        for(int i = 0; i < Seq_length(memory->mapped); i++){
                void* curr = Seq_get(memory->mapped, i);
                if(curr == NULL){

                }
                else{
                        printf(" %d ", i);
                }
        }
        printf("\n");
}


#undef T

