#pragma once

#include <sys/types.h>
#include <thread>

#define QUEUE_MAX_SIZE 100000
extern "C" {
#include "libavformat/avformat.h"
}

typedef AVPacket* NodeElement;

typedef struct _Node {
    NodeElement data;
    struct _Node* next;
} Node;

typedef struct _Queue{
    int size;
    Node* head;
    Node* tail;
    bool is_block;
    std::string name;
    std::mutex mutex;
    std::condition_variable not_empty_cv;
    std::condition_variable not_full_cv;
}Queue;

void queue_init(Queue* queue, const char* name);

void queue_destroy(Queue* queue);

bool queue_is_empty(Queue* queue);

bool queue_is_full(Queue* queue);

void queue_in(Queue* queue, NodeElement element);

NodeElement queue_out(Queue* queue);

void queue_clear(Queue* queue);

void break_block(Queue* queue);
