#pragma once

#include <sys/types.h>

#define QUEUE_MAX_SIZE 50

typedef int NodeElement;

typedef struct _Node {
	NodeElement data;
	struct _Node* next;
} Node;

typedef struct _Queue{
	int size;
	Node* head;
	Node* tail;
}Queue;

void queue_init(Queue* queue);

void queue_destroy(Queue* queue);

bool queue_is_empty(Queue* queue);

bool queue_is_full(Queue* queue);

void queue_in(Queue* queue, NodeElement element);

NodeElement queue_out(Queue* queue);