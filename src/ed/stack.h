
#ifndef _STACK_H_
#define _STACK_H_

#include "types.h"

typedef struct Stack Stack;

Stack *stack_construct(size_t smemb, destructor_fn destructor);

void stack_push(Stack *stack, void *data);

bool stack_empty(Stack *stack);

void *stack_pop(Stack *stack);

void stack_destroy(Stack *stack);

#endif