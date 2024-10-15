#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef void *(*allocate)(size_t size);
typedef void (*deallocate)(void *data);
typedef struct Arena
{
    struct Arena *next_reg;
    allocate allocate_func;
    deallocate deallocate_func;
    size_t alignment;
    size_t capacity;
    size_t size;
    uint8_t *data;
} Arena;

#ifndef RMEMDEF
    #ifndef RMEM_STATIC
        #define RMEMDEF extern
    #else
        #define RMEMDEF static
    #endif
#endif

//NOTE: If no custom allocator, pass NULL for both allocate and deallocate
RMEMDEF Arena arena_init(size_t capacity, size_t alignment, allocate allocate_func, deallocate deallocate_func);
RMEMDEF void *arena_alloc(Arena *self, size_t size);
inline RMEMDEF void arena_clear(Arena *self);
RMEMDEF void arena_destroy(Arena *self);

#ifdef RARENA_IMPLEMENTATION
RMEMDEF Arena arena_init(size_t capacity, size_t alignment, allocate allocate_func, deallocate deallocate_func)
{
    Arena result = {0};
    result.allocate_func = allocate_func ? allocate_func : malloc; 
    result.deallocate_func = deallocate_func ? deallocate_func : free;
    result.alignment = (alignment > 0) ? alignment : 1;
    result.capacity = capacity;
    result.data = result.allocate_func(sizeof(uint8_t) * capacity);
    if (result.data == NULL)
        assert(0 && "Failed to allocate memory!");
    assert((self->alignment & (self->alignment - 1)) == 0 && "Alignment must be a power of 2.");

    return result;
}

static size_t align_(Arena *self)
{
    return (self->size + self->alignment - 1) & ~(self->alignment - 1); 
}

RMEMDEF void *arena_alloc(Arena *self, size_t size)
{
    //TODO: Add overflow handling
    size_t current_offset = align_(self);
    assert(current_offset + size <= self->capacity);
    self->size += current_offset + size;
    return &self->data[current_offset];
}

inline RMEMDEF void arena_clear(Arena *self)
{
    self->size = 0;
}

RMEMDEF void arena_destroy(Arena *self)
{
    self->capacity = 0;
    self->size = 0;
    if (self->data != NULL)
        self->deallocate_func(self->data);
}
#endif