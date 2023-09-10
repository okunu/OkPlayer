#include "Queue.h"

void queue_init(Queue* queue) {
	queue->size = 0;
	queue->head = nullptr;
	queue->tail = nullptr;
}

void queue_destroy(Queue* queue) {
	Node* tmp = queue->head;
	while (tmp != nullptr){
		queue->head = queue->head->next;
		delete(tmp);
		tmp = queue->head;
	}
	queue->head = nullptr;
	queue->tail = nullptr;
	queue->size = 0;
}

bool queue_is_empty(Queue* queue) {
	if (queue->size == 0) {
		return true;
	}
	return false;
}

bool queue_is_full(Queue* queue) {
	return queue->size == QUEUE_MAX_SIZE;
}

void queue_in(Queue* queue, NodeElement element) {
	if (queue->size == QUEUE_MAX_SIZE) {
		return;
	}
	Node* node = new Node;
	node->data = element;
	node->next = nullptr;
	if (queue->head == nullptr) {
		queue->head = node;
		queue->tail = node;
	} else {
		queue->tail->next = node;
		queue->tail = node;
	}
	queue->size++;
}

NodeElement queue_out(Queue* queue) {
	if (queue->size == 0) {
		return NULL;
	}
	Node* tmp = queue->head;
	queue->head = queue->head->next;
	auto value = tmp->data;
	delete(tmp);
	queue->size--;
	return value;
}

