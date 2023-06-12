#ifndef HEAP_H
#define HEAP_H

#include <stdlib.h>
#include <stdint.h>

struct heap_t{
	struct block_t* head;
	struct block_t* tail;
	void* mem_start;
};

struct block_t{
	uint32_t checksum;
	struct block_t* prev;
	struct block_t* next;

	// negative mem_state means free, positive means allocated
	int mem_state;
	int mem_to_show;
};

enum pointer_type_t
{
    pointer_null, // 0
    pointer_heap_corrupted, // 1
    pointer_control_block, // 2
    pointer_inside_fences, // 3
    pointer_inside_data_block, // 4
    pointer_unallocated, // 5
    pointer_valid // 6
};

int heap_setup(void);
void heap_clean(void);

void* heap_malloc(size_t size);                             
void* heap_calloc(size_t number, size_t size);              
void* heap_realloc(void* memblock, size_t count);               
void  heap_free(void* memblock);

size_t   heap_get_largest_used_block_size(void);
enum pointer_type_t get_pointer_type(const void* const pointer);

int heap_validate(void);

void* heap_malloc_aligned(size_t count);                     
void* heap_calloc_aligned(size_t number, size_t size);       
void* heap_realloc_aligned(void* memblock, size_t size);

#endif