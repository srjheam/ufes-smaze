
#ifndef _DEQUE_H_
#define _DEQUE_H_

#include "stdlib.h"

#include "types.h"

typedef struct Deque Deque;

Deque *deque_construct(size_t smemb, destructor_fn destructor);

/**
 * @brief Pushes an element to the back of the deque.
 * 
 * @param d The deque to push to.
 * @param val The address to the value to push. If it is a pointer, its plain address will be pushed.
 */
void deque_push_back(Deque *d, void *val);

/**
 * @brief Pushes an element to the front of the deque.
 * 
 * @param d The deque to push to.
 * @param val The address to the value to push. If it is a pointer, its plain address will be pushed.
 */
void deque_push_front(Deque *d, void *val);

/**
 * @brief Pops an element from the back of the deque.
 * 
 * @param d The deque to pop from.
 * @return void* The popped value. A heap allocated address to the value is returned and shall be freed by the user.
 */
void *deque_pop_back(Deque *d);

/**
 * @brief Pops an element from the front of the deque.
 * 
 * @param d The deque to pop from.
 * @return void* The popped value. A heap allocated address to the value is returned and shall be freed by the user.
 */
void *deque_pop_front(Deque *d);

int deque_size(Deque *d);

void *deque_get(Deque *d, int idx);

void *deque_iterator_forward(Deque *d, int *saveptr);

void deque_destroy(Deque *d);

#endif