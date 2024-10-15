#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

typedef struct arena
{
    size_t capacity;
    size_t size;
    uint8_t *data;
} arena;

arena arena_init(size_t capacity)
{
    arena result = {0};
    result.capacity = capacity;
    result.data = malloc(sizeof(uint8_t) * capacity);
    return result;
}

void arena_alloc(arena *self, size_t size)
{
    assert(self->size + size <= self->capacity);
    self->size += size;
    return &data[self->size];
}

void arena_clear(arena *self)
{
    self->size = 0;
}

void arena_destroy(arena *self)
{
    self->capacity = 0;
    self->size = 0;
    if (self->data != NULL)
        free(self->data);
}