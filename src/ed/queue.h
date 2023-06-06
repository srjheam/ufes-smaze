
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "types.h"

typedef struct Queue Queue;

Queue *queue_construct(size_t smemb, copy_fn copy, destructor_fn destructor);

void queue_push(Queue *queue, void *data);

bool queue_empty(Queue *queue);

void *queue_pop(Queue *queue);

void queue_destroy(Queue *queue);

#endif