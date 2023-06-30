#include "stdlib.h"
#include "string.h"

#include "black_magic.h"

#include "deque.h"

// Of course I didn't have any idea of the chunk size
// https://stackoverflow.com/q/57031917/12511877
// https://stackoverflow.com/a/4089128/12511877
//
// With this approach, the chunk length is determined by the size of the
// element. If the element size (smemb) is greater than 8 bytes, then the chunk
// length is 1, which turns the deque into a convoluted list with performance
// penalties in the majority of cases. Honestly, Clang's implementation is
// better but at this point I'm too afraid to switch.
// https://stackoverflow.com/q/57031917
#define _DEQUE_CHUNKLEN(_smemb)                                                \
    (_smemb <= 1   ? 16                                                        \
     : _smemb <= 2 ? 8                                                         \
     : _smemb <= 4 ? 4                                                         \
     : _smemb <= 8 ? 2                                                         \
                   : 1) /* elements per chunk */

// Chunk byte size
#define _DEQUE_CHUNKSIZ(_smemb) (_smemb <= 8 ? 16 : _smemb)

#define _DEQUE_CHUNKINI 4

//      lback
// ...  |
// *    V
// *    *
// * ****
// * **
// *  ^
// *  |
// *  hfront
// *
// ...
//
// _DEQUE_LCHUNK_COMPLEMENT_BSIZ = 12 (3 * _DEQUE_CHUNKLEN)
// _DEQUE_LCHUNK_BSIZ            = 4  (1 * _DEQUE_CHUNKLEN)
//
// _DEQUE_HCHUNK_BSIZ            = 8 (2 * _DEQUE_CHUNKLEN)
// _DEQUE_HCHUNK_COMPLEMENT_BSIZ = 8 (2 * _DEQUE_CHUNKLEN)
//
#define _DEQUE_LCHUNK_COMPLEMENT_BSIZ(_d) (_d->lback - _d->lchunk[0])
#define _DEQUE_LCHUNK_BSIZ(_d)                                                 \
    (_DEQUE_CHUNKSIZ(_d->smemb) - _DEQUE_LCHUNK_COMPLEMENT_BSIZ(_d))

#define _DEQUE_HCHUNK_BSIZ(_d)                                                 \
    (_d->hfront - _d->lchunk[_d->nchunks - 1] + _d->smemb)
#define _DEQUE_HCHUNK_COMPLEMENT_BSIZ(_d)                                      \
    (_DEQUE_CHUNKSIZ(_d->smemb) - _DEQUE_HCHUNK_BSIZ(_d))

//
// _DEQUE_LCHUNK_COMPLEMENT_LEN = 3
// _DEQUE_LCHUNK_LEN            = 1
//
// _DEQUE_HCHUNK_LEN            = 2
// _DEQUE_HCHUNK_COMPLEMENT_LEN = 2
//
#define _DEQUE_LCHUNK_COMPLEMENT_LEN(_d)                                       \
    (_DEQUE_CHUNKSIZ(_d->smemb) > 16                                           \
         ? 1                                                                   \
         : _DEQUE_LCHUNK_COMPLEMENT_BSIZ(_d) >> log2_pow2(_d->smemb))
#define _DEQUE_LCHUNK_LEN(_d)                                                  \
    (_DEQUE_CHUNKLEN(_d->smemb) - _DEQUE_LCHUNK_COMPLEMENT_LEN(_d))

#define _DEQUE_HCHUNK_LEN(_d)                                                  \
    (_DEQUE_CHUNKSIZ(_d->smemb) > 16                                           \
         ? 1                                                                   \
         : _DEQUE_HCHUNK_BSIZ(_d) >> log2_pow2(_d->smemb))
#define _DEQUE_HCHUNK_COMPLEMENT_LEN(_d)                                       \
    (_DEQUE_CHUNKLEN(_d->smemb) - _DEQUE_HCHUNK_LEN(_d))
//
// e.g
//
//     lback
//     |
//     V
// *   **
// * ****
// * ****
// * ****
// * ****
// * ****
// * ****
// * ***
// ^   ^
// |   |
// |   hfront
// lchunk
//
// nchunks = 8
// smemb = 4
// _DEQUE_CHUNKSIZ(smemb) = 4
//
struct Deque {
    byte **chunks;   /// address to the first chunk in the array of chunks
    byte **lchunk;   /// address to the low chunk in the array
    size_t nchunks;  /// number of chunks
    size_t capacity; /// capacity of chunks array
    byte
        *hfront; /// pointer to the **true** front, high memory address element;
                 /// otherwise, it is NULL when there are no elements at all
    byte *lback; /// pointer to the **true** back, low memory address element;
                 /// otherwise, it is NULL when there are no elements at all
    size_t smemb;             /// size of each element
    destructor_fn destructor; /// function to destroy each element
};

void __deque_growth(Deque *d) {
    // If the chunks array is full, then double its size;
    // otherwise, move the chunks to the middle of the array
    //
    // I think this may cause an excess of memory move, but growing the
    // chunks capacity everytime doesn't seem fine given that I may use the
    // deque always as a queue or as a stack and in this cases I'll be wasting
    // at least half of the memory
    if (d->nchunks >= d->capacity >> 1) {
        size_t diff = d->lchunk - d->chunks;

        d->capacity <<= 1;
        d->chunks = realloc(d->chunks, __SIZEOF_POINTER__ * d->capacity);

        d->lchunk = (byte **)((byte *)d->chunks + diff);
    }

    byte **nlchunk = d->chunks + ((d->capacity - d->nchunks) >> 1);

    memmove(nlchunk, d->lchunk, d->nchunks * __SIZEOF_POINTER__);

    d->lchunk = nlchunk;
}

Deque *deque_construct(size_t smemb, destructor_fn destructor) {
    Deque *d = malloc(sizeof *d);

    d->smemb = smemb;

    d->nchunks = 0;
    d->capacity = _DEQUE_CHUNKINI;
    d->chunks = malloc(__SIZEOF_POINTER__ * d->capacity);

    d->lchunk = d->chunks + (d->capacity >> 1);
    d->hfront = NULL;
    d->lback = NULL;

    d->destructor = destructor;

    return d;
}

void deque_push_back(Deque *d, void *val) {
    if (d->nchunks == 0) {
        d->nchunks = 1;
        d->lchunk[0] = malloc(_DEQUE_CHUNKSIZ(d->smemb));

        d->hfront = d->lchunk[0];
        d->lback = d->lchunk[0];

        memcpy(d->lback, val, d->smemb);

        return;
    }

    if (_DEQUE_LCHUNK_COMPLEMENT_LEN(d) == 0) {
        if (d->lchunk == d->chunks)
            __deque_growth(d);

        d->lchunk--;
        d->nchunks++;

        d->lchunk[0] = malloc(_DEQUE_CHUNKSIZ(d->smemb));

        d->lback = d->lchunk[0] + (_DEQUE_CHUNKSIZ(d->smemb) - d->smemb);
    } else
        d->lback -= d->smemb;

    memcpy(d->lback, val, d->smemb);
}

void deque_push_front(Deque *d, void *val) {
    if (d->nchunks == 0) {
        d->nchunks = 1;
        d->lchunk[0] = malloc(_DEQUE_CHUNKSIZ(d->smemb));

        d->hfront = d->lchunk[0];
        d->lback = d->lchunk[0];

        memcpy(d->lback, val, d->smemb);

        return;
    }

    if (_DEQUE_HCHUNK_COMPLEMENT_LEN(d) == 0) {
        if (d->lchunk[d->nchunks - 1] == d->chunks[d->capacity - 1])
            __deque_growth(d);

        d->lchunk[d->nchunks++] = malloc(_DEQUE_CHUNKSIZ(d->smemb));

        d->hfront = d->lchunk[d->nchunks - 1];
    } else
        d->hfront += d->smemb;

    memcpy(d->hfront, val, d->smemb);
}

void *deque_pop_back(Deque *d) {
    if (deque_size(d) == 0)
        return NULL;

    void *ret = malloc(d->smemb);
    memcpy(ret, d->lback, d->smemb);
    if (d->destructor)
        d->destructor(d->lback);

    if (_DEQUE_LCHUNK_LEN(d) == 1 || d->lback == d->hfront) {
        free(d->lchunk[0]);
        d->nchunks--;

        if (d->nchunks > 0) {
            d->lchunk++;
            d->lback = d->lchunk[0];
        }
    } else
        d->lback += d->smemb;

    return ret;
}

void *deque_pop_front(Deque *d) {
    if (deque_size(d) == 0)
        return NULL;

    void *ret = malloc(d->smemb);
    memcpy(ret, d->hfront, d->smemb);
    if (d->destructor)
        d->destructor(d->hfront);

    if (_DEQUE_HCHUNK_LEN(d) == 1 || d->lback == d->hfront) {
        free(d->lchunk[d->nchunks - 1]);
        d->nchunks--;

        if (d->nchunks > 0)
            d->hfront = d->lchunk[d->nchunks - 1] +
                        (_DEQUE_CHUNKSIZ(d->smemb) - d->smemb);
    } else
        d->hfront -= d->smemb;

    return ret;
}

int deque_size(Deque *d) {
    if (d->nchunks == 0)
        return 0;

    return d->nchunks * _DEQUE_CHUNKLEN(d->smemb) -
           (_DEQUE_LCHUNK_COMPLEMENT_LEN(d) - _DEQUE_HCHUNK_COMPLEMENT_LEN(d));
}

void *deque_get(Deque *d, int idx) {
    if (idx < 0 || idx >= deque_size(d))
        return NULL;

    if (_DEQUE_CHUNKLEN(d->smemb) == 1)
        return d->lchunk[idx];

    // Stolen from
    // https://codereview.stackexchange.com/a/254925

    const int logsiz = log2_pow2(_DEQUE_CHUNKLEN(d->smemb));

    // position relative to the low chunk of the deque
    int pos = _DEQUE_LCHUNK_COMPLEMENT_LEN(d) + idx;
    byte *tchunk = d->lchunk[pos >> logsiz];
    int telem = pos & (_DEQUE_CHUNKLEN(d->smemb) - 1);

    return tchunk + telem * d->smemb;
}

void *deque_iterator_forward(Deque *d, int *saveptr) {
    if (saveptr == NULL || *saveptr >= deque_size(d))
        return NULL;

    return deque_get(d, (*saveptr)++);
}

void deque_destroy(Deque *d) {
    if (d->destructor) {
        int i = 0;
        void *curr;
        while ((curr = deque_iterator_forward(d, &i)))
            d->destructor(curr);
    }

    while (d->nchunks-- > 0)
        free(d->lchunk[d->nchunks]);

    free(d->chunks);
    free(d);
}
