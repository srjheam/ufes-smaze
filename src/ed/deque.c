#include "deque.h"

#include "black_magic.h"

// Of course I didn't have any idea of the chunk size
// https://stackoverflow.com/q/57031917/12511877
// https://stackoverflow.com/a/4089128/12511877
#define _DEQUE_CHUNKSIZ(_smemb)                                                \
    (_smemb <= 1   ? 16                                                        \
     : _smemb <= 2 ? 8                                                         \
     : _smemb <= 4 ? 4                                                         \
     : _smemb <= 8 ? 2                                                         \
                   : 1) /* elements per block */

#define _DEQUE_BLOCKSIZ(_smemb) (_smemb <= 8 ? 16 : _smemb)

#define _DEQUE_CHUNKINI 4

#define _DEQUE_LCHUNK_COMPLEMENT_BLEN(_d) (_d->lback - _d->lchunk[0])
#define _DEQUE_LCHUNK_BLEN(_d)                                                 \
    (_DEQUE_CHUNKSIZ(_d->smemb) - _DEQUE_LCHUNK_COMPLEMENT_BLEN(_d))

#define _DEQUE_HCHUNK_BLEN(_d) (_d->lchunk[_d->nchunks - 1] - _d->hfront)
#define _DEQUE_HCHUNK_COMPLEMENT_BLEN(_d)                                      \
    (_DEQUE_CHUNKSIZ(_d->smemb) - _DEQUE_HCHUNK_BLEN(_d))

//
// e.g
//
//    hfront
//    |
//    V
// * **
// * ****
// * ****
// * ****
// * ****
// * ****
// * ****
// *  ***
// ^  ^
// |  |
// |  lback
// lchunk
//
// nchunks = 8
// smemb = 4
// _DEQUE_BLOCKSIZ(smemb) = 4
//
struct Deque {
    byte **chunks;            /// array of chunks
    byte **lchunk;            /// low chunk
    size_t nchunks;           /// number of chunks
    size_t capacity;          /// capacity of chunks array
    byte *hfront;             /// pointer to the front, high element
    byte *lback;              /// pointer to the back, low element
    size_t smemb;             /// size of each element
    destructor_fn destructor; /// function to destroy each element
    copy_fn copy;             /// function to copy each element
};

Deque *deque_construct(size_t smemb, copy_fn copy, destructor_fn destructor) {
    Deque *d = malloc(sizeof *d);

    d->smemb = smemb;

    d->nchunks = 1;
    d->capacity = _DEQUE_CHUNKINI;
    d->chunks = malloc(__SIZEOF_POINTER__ * d->capacity);
    d->chunks[d->capacity >> 1] =
        malloc(sizeof(byte) * smemb * _DEQUE_CHUNKSIZ(smemb));
    d->lchunk = d->chunks + (d->capacity >> 1) * __SIZEOF_POINTER__;
    d->hfront = d->lchunk[0];
    d->lback = d->lchunk[0];

    d->destructor = destructor;
    d->copy = copy;

    return d;
}

void deque_push_back(Deque *d, void *val) {}

void deque_push_front(Deque *d, void *val) {}

void *deque_pop_back(Deque *d) {
    if (deque_size(d) == 0)
        return NULL;
}

void *deque_pop_front(Deque *d) {
    if (deque_size(d) == 0)
        return NULL;

    void *ret = d->copy(d->hfront);
    d->destructor(d->hfront);

    if (d->lchunk[0] != d->chunks[0] && _DEQUE_HCHUNK_BLEN(d) > 0) {
        d->hfront -= d->smemb;
        free(d->lchunk + (d->nchunks - 1) * __SIZEOF_POINTER__);
        d->lchunk--;
        d->nchunks--;
        d->hfront = d->lchunk[d->nchunks - 1] +
                    (_DEQUE_CHUNKSIZ(d->smemb) - 1) * d->smemb;
    }

    return ret;
}

int deque_size(Deque *d) {
    if (d->nchunks == 0)
        return 0;

    return d->nchunks * _DEQUE_CHUNKSIZ(d->smemb) -
           _DEQUE_LCHUNK_COMPLEMENT_BLEN(d) - _DEQUE_HCHUNK_COMPLEMENT_BLEN(d);
}

void *deque_get(Deque *d, int idx) {
    if (idx < 0 || idx >= deque_size(d))
        return NULL;

    if (_DEQUE_CHUNKSIZ(d->smemb) == 1)
        return d->lchunk[idx];

    // Stolen from
    // https://codereview.stackexchange.com/a/254925

    const int logsiz = log2_pow2(_DEQUE_CHUNKSIZ(d->smemb));

    // position relative to the low chunk of the deque
    int pos = log2_pow2(_DEQUE_LCHUNK_COMPLEMENT_BLEN(d)) + idx;
    byte **tchunk = d->lchunk[pos >> logsiz];
    int telem = pos & (_DEQUE_CHUNKSIZ(d->smemb) - 1);

    return tchunk[telem];
}

void *deque_iterator_forward(Deque *d, int *saveptr) {
    if (saveptr == NULL || *saveptr >= deque_size(d))
        return NULL;

    return deque_get(d, (*saveptr)++);
}

void deque_destroy(Deque *d) {
    int i = 0;
    void *curr;
    while ((curr = deque_iterator_forward(d, &i)))
        if (d->destructor)
            d->destructor(curr);

    free(d->chunks);
    free(d);
}
