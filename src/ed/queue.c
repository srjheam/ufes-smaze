
#include "deque.h"

#include "queue.h"

struct Queue {
    Deque *deque;
};

Queue *queue_construct(size_t smemb, destructor_fn destructor) {
    Queue *queue = malloc(sizeof(Queue));

    queue->deque = deque_construct(smemb, destructor);

    return queue;
}

void queue_push(Queue *queue, void *data) {
    deque_push_front(queue->deque, data);
}

bool queue_empty(Queue *queue) {
    return deque_size(queue->deque) == 0;
}

void *queue_pop(Queue *queue) {
    return deque_pop_back(queue->deque);
}

void queue_destroy(Queue *queue) {
    deque_destroy(queue->deque);
    free(queue);
}
