#include "deque.h"

#include "stack.h"

struct Stack {
    Deque *deque;
};

Stack *stack_construct(size_t smemb, destructor_fn destructor) {
    Stack *stack = malloc(sizeof(Stack));

    stack->deque = deque_construct(smemb, destructor);

    return stack;
}

void stack_push(Stack *stack, void *data) {
    deque_push_front(stack->deque, data);
}

bool stack_empty(Stack *stack) { return deque_size(stack->deque) == 0; }

void *stack_pop(Stack *stack) { return deque_pop_front(stack->deque); }

void stack_destroy(Stack *stack) {
    deque_destroy(stack->deque);
    free(stack);
}
