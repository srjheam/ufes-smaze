
#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "stdlib.h"

#include "types.h"

typedef struct Deque Deque;

// criacao do deque
Deque *deque_construct(size_t smemb, destructor_fn destructor);

// funcoes para insercao na d--ireita e esquerda (devem ser feitas em O(1), com eventual realloc)
void deque_push_back(Deque *d, void *val);
void deque_push_front(Deque *d, void *val);

// funcoes para remocao na direita e esquerda (devem ser feitas em O(1))
void *deque_pop_back(Deque *d);
void *deque_pop_front(Deque *d);

// numero de elementos
int deque_size(Deque *d);

// obtem o i-esimo elemento
void *deque_get(Deque *d, int idx);

void *deque_iterator_forward(Deque *d, int *saveptr);

// libera o espaco alocado para o deque
void deque_destroy(Deque *d);

#endif