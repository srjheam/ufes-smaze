#ifndef _KVP_H_
#define _KVP_H_

typedef struct Kvp Kvp;

Kvp *kvp_construct(void *key, void *value);

void *kvp_key(Kvp *kvp);

void *kvp_value(Kvp *kvp);

void kvp_set_key(Kvp *kvp, void *key);

void kvp_set_value(Kvp *kvp, void *value);

void kvp_destroy(Kvp *kvp);

#endif