#pragma once

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

typedef struct Pool
{
    size_t chunk_size;
    size_t total_chunks;
    uint8_t *data;
    void **freelist;
} Pool;

Pool pool_init(size_t chunk_size, size_t total_chunks)
{
    Pool result = {0};
    result.chunk_size = chunk_size;
    result.total_chunks = total_chunks;

    //TODO: Add error handling
    result.data = (uint8_t *)malloc(chunk_size * total_chunks);
    result.freelist = (void **)malloc(total_chunks * sizeof(void *));

    for (size_t i = 0; i < total_chunks; i++)
        result.freelist[i] = result.data + chunk_size * i;
    return result;
}

void *pool_alloc(Pool *self)
{
    if (self->total_chunks == 0)
        return NULL;
    return self->freelist[--self->total_chunks];
}

void pool_free(Pool *self, void *chunk)
{
    self->freelist[self->total_chunks++] = chunk;
}

void pool_destroy(Pool *self)
{
    free(self->data);
    free(self->freelist);
    self->data = NULL;
    self->freelist = NULL;
    self->chunk_size = 0;
    self->total_chunks = 0;
}