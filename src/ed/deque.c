
#include "deque.h"

#include "types.h"

// Of course I didn't have any idea of the chunk size
// https://stackoverflow.com/q/57031917/12511877
// https://stackoverflow.com/a/4089128/12511877
#define _DEQUE_CHUNKSIZ(_smemb)                                                \
    (_smemb <= 1   ? 16                                                        \
     : _smemb <= 2 ? 8                                                         \
     : _smemb <= 4 ? 4                                                         \
     : _smemb <= 8 ? 2                                                         \
                   : 1) /* elements per block */

#define _DEQUE_CHUNKINI 4

struct Deque {
    byte **chunks;            /// arrray of chunks
    byte *fchunk;             /// first chunk
    int nchunks;              /// number of chunks
    int capacity;             /// capacity of chunks array
    byte *front;              /// pointer to the front element
    byte *back;               /// pointer to the back element
    size_t smemb;             /// size of each element
    destructor_fn destructor; /// function to destroy each element
};

Deque *deque_construct(size_t smemb, destructor_fn destructor) {
    Deque *d = malloc(sizeof *d);

    d->smemb = smemb;

    d->nchunks = 0;
    d->capacity = _DEQUE_CHUNKINI;
    d->chunks = malloc(sizeof(byte *) * d->capacity);
    d->fchunk = malloc(sizeof(byte) * smemb * _DEQUE_CHUNKSIZ(smemb));
    d->chunks[d->capacity >> 1] = d->fchunk;
    d->front = d->fchunk;
    d->back = d->fchunk;

    d->destructor = destructor;

    return d;
}

void deque_push_back(Deque *d, void *val) {}

void deque_push_front(Deque *d, void *val) {}

void *deque_pop_back(Deque *d) {}

void *deque_pop_front(Deque *d) {}

int deque_size(Deque *d) {
    return d->nchunks * _DEQUE_CHUNKSIZ(d->smemb) +
           (d->back - d->front) / d->smemb;
}

void *deque_get(Deque *d, int idx) {
    if (idx < 0 || idx >= deque_size(d)) {
        return NULL;
    }

    int chunk_idx = idx / _DEQUE_CHUNKSIZ(d->smemb);
    int elem_idx = idx % _DEQUE_CHUNKSIZ(d->smemb);

    return d->chunks[chunk_idx] + elem_idx * d->smemb;
}

void deque_destroy(Deque *d) {
    free(d->chunks);
    free(d);
}
