#include <math.h>
#include <stdbool.h>

#include "utils.h"

size_t utils_int_pow_mod(size_t base, size_t exp, size_t mod) {
    // mod(a ^ d, n)
    // stolen
    // https://stackoverflow.com/a/8498251

    base %= mod;
    size_t x = 1;
    while (exp > 0) {
        if (exp & 1)
            x = (x * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    return x;
}

bool __utils_is_prime_miller_rabin(size_t n, size_t k) {
    // stolen
    // https://chat.openai.com/share/0d635ac7-dd57-4dca-84d4-8f8060492fb4

    if (n <= 3)
        return n == 2 || n == 3;
    if (n % 2 == 0)
        return false;

    // Write n-1 as 2^r * d, where d is odd
    size_t r = 0;
    size_t d = n - 1;
    while (d % 2 == 0) {
        r += 1;
        d /= 2;
    }

    // Run Miller - Rabin primality test k times
    size_t i;
    for (i = 0; i < k; i++) {
         size_t a = (size_t)rand() % (n - 3) + 2;

        size_t x = utils_int_pow_mod(a, d, n);
        if (x == 1 || x == n - 1)
            continue;

        size_t j = 0;
        for (size_t j = 0; j < r - 1; j++) {
            x = utils_int_pow_mod(x, 2, n);
            if (x == 1)
                return false;
            if (x == n - 1)
                break;
        }

        if (j == r - 1)
            return false;
    }

    return true;
}

size_t __utils_find_near_next_prime(size_t n) {
    // stolen
    // https://chat.openai.com/share/0d635ac7-dd57-4dca-84d4-8f8060492fb4

    if (n <= 2)
        return 2;

    size_t prime = n;
    while (true) {
        if (__utils_is_prime_miller_rabin(prime, 10))
            return prime;

        prime++;
    }
}

size_t utils_neat_ht_initial_capacity_estimator(size_t maxItems) {
    // stolen from
    // https://github.com/srjheam/ufes-nlprog/blob/8ab3ff252e1ee32d167ad0f683046250a1cba867/common/src/hash_table.c#L83

    // desired load factor of .75
    size_t targetInitialCapacity = maxItems - (maxItems >> 2);

    // try to get next prime number
    return __utils_find_near_next_prime(targetInitialCapacity);
}
