#include "black_magic.h"

#include <stdint.h>

// https://graphics.stanford.edu/~seander/bithacks.html#IntegerLogDeBruijn
int log2_pow2(unsigned int v)
{
    static const int MultiplyDeBruijnBitPosition2[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9};

    return MultiplyDeBruijnBitPosition2[(uint32_t)(v * 0x077CB531U) >> 27];
}
