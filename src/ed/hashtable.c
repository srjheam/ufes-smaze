#include "exceptions.h"
#include "key_value_pair.h"

#include "hashtable.h"

#define HASH(h, key) (h->hash(key) % h->bucketsCapacity)

typedef struct _HtNode _HtNode;

struct _HtNode {
    void *key;
    void *val;
    struct _HtNode *next;
};

struct Hashtable {
    size_t bucketsCapacity;
    size_t bucketsCount;
    _HtNode **buckets;

    hash_fn hash;
    cmp_fn keyCmp;

    copy_fn keyCopy;

    destructor_fn keyDestructor;
    destructor_fn valDestructor;
};

Hashtable *ht_construct(size_t initalCapacity, float loadFactor, hash_fn hash,
                        cmp_fn keyCmp, copy_fn keyCopy,
                        destructor_fn keyDestructor,
                        destructor_fn valDestructor) {
    if (loadFactor <= 0 || loadFactor > 1)
        exception_throw_argument("loadFactor must be in (0, 1]");

    Hashtable *h = malloc(sizeof(Hashtable));

    h->bucketsCapacity = initalCapacity;
    h->bucketsCount = 0;
    h->buckets = calloc(initalCapacity, sizeof(_HtNode *));

    h->hash = hash;
    h->keyCmp = keyCmp;

    h->keyCopy = keyCopy;

    h->keyDestructor = keyDestructor;
    h->valDestructor = valDestructor;

    return h;
}

void ht_put(Hashtable *h, void *key, void *val) {
    size_t iBucket = HASH(h, key);

    _HtNode *node = h->buckets[iBucket];
    if (node == NULL)
        h->bucketsCount++;

    while (node != NULL) {
        if (h->keyCmp(node->key, key) == 0) {
            if (h->valDestructor != NULL)
                h->valDestructor(node->val);

            node->val = val;
            return;
        }
        node = node->next;
    }

    _HtNode *nnode = malloc(sizeof(_HtNode));
    nnode->key = h->keyCopy(key);
    nnode->val = val;
    nnode->next = h->buckets[iBucket];
    h->buckets[iBucket] = nnode;
}

void *ht_lookup(Hashtable *h, void *key) {
    size_t iBucket = HASH(h, key);

    _HtNode *node = h->buckets[iBucket];
    while (node != NULL) {
        if (h->keyCmp(node->key, key) == 0)
            return node->val;

        node = node->next;
    }

    // it should throw an error when the key is not found because NULL may be a
    // valid value when the hash table is used to store pointers
    // but for the sake of simplicity, this will return NULL
    return NULL;
}

void *ht_delete(Hashtable *h, void *key) {
    size_t iBucket = HASH(h, key);

    _HtNode *node = h->buckets[iBucket];
    _HtNode *prev = NULL;
    while (node != NULL) {
        if (h->keyCmp(node->key, key) == 0) {
            if (prev == NULL) {
                h->buckets[iBucket] = node->next;
                if (h->buckets[iBucket] == NULL)
                    h->bucketsCount--;
            }
            else
                prev->next = node->next;

            if (h->keyDestructor != NULL)
                h->keyDestructor(node->key);

            void *val = node->val;
            free(node);
            return val;
        }

        prev = node;
        node = node->next;
    }

    // it should throw an error when the key is not found because NULL may be a
    // valid value when the hash table is used to store pointers
    // but for the sake of simplicity, this will return NULL
    return NULL;
}

struct HashtableIterator
{
    Hashtable *ht;
    size_t countBucket;
    size_t currBucket;
    _HtNode *curr;
};

HashtableIterator *htit_begin(Hashtable *ht) {
    HashtableIterator *it = malloc(sizeof *it);

    it->ht = ht;
    it->countBucket = 0;
    it->currBucket = -1;
    it->curr = NULL;

    return it;
}

Kvp *htit_next(HashtableIterator *it) {
    if (it->curr == NULL)
        it->countBucket++;
    else
        it->curr = it->curr->next;

    Kvp *kvp = NULL;
    while (it->curr == NULL || it->curr->next == NULL)
        it->curr = it->ht->buckets[++it->currBucket];

    return kvp;
}

bool htit_is_joever(HashtableIterator *it) {
    return it->ht->bucketsCount == 0 || (it->curr->next == NULL && it->countBucket == it->ht->bucketsCount);
}

void htit_destroy(HashtableIterator *it) {
    free(it);
}

void ht_clear(Hashtable *h) {
    size_t bucketc = 0;
    for (size_t i = 0; i < h->bucketsCapacity && bucketc < h->bucketsCount; i++) {
        _HtNode *node = h->buckets[i];
        if (node != NULL)
            bucketc++;

        while (node != NULL) {
            _HtNode *next = node->next;

            if (h->keyDestructor != NULL)
                h->keyDestructor(node->key);

            if (h->valDestructor != NULL)
                h->valDestructor(node->val);

            free(node);
            h->buckets[i] = NULL;
            node = next;
        }
    }

    h->bucketsCount = 0;
}

void ht_destroy(Hashtable *h) {
    ht_clear(h);
    free(h->buckets);
    free(h);
}
