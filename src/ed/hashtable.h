
#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <stdlib.h>

#include "types.h"

typedef struct Hashtable Hashtable;

typedef struct HashtableIterator HashtableIterator;

/**
 * @brief Initializes a new, empty instance of the Hashtable using the specified
 * initial capacity, load factor, hash code provider, and comparer.
 *
 * @param initalCapacity The initial number of buckets that the Hashtable can
 * initially contain.
 * @param loadFactor The load factor of the Hashtable. Must be between 0.1
 * and 1.0, inclusive.
 * @param hash The hash function to use to hash keys.
 * @param keyCmp The key comparison function to use to compare keys for
 * equality.
 * @param keyDestructor The destructor function to use to destroy keys.
 * @param valDestructor The destructor function to use to destroy values.
 * @return Hashtable* A new, empty instance of the Hashtable class.
 *
 * @remarks The hash table currently does not grow in size when the number of
 * elements exceeds the load factor. Therefore, the load factor as a construct
 * argument is currently ignored.
 */
Hashtable *ht_construct(size_t initalCapacity, float loadFactor, hash_fn hash,
                        cmp_fn keyCmp, copy_fn keyCopy,
                        destructor_fn keyDestructor,
                        destructor_fn valDestructor);

void ht_put(Hashtable *h, void *key, void *val);

void *ht_lookup(Hashtable *h, void *key);

void *ht_delete(Hashtable *h, void *key);

void ht_clear(Hashtable *h);

void ht_destroy(Hashtable *h);

#endif