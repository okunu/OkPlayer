#include "Queue.h"
#include "LogUtil.h"

void queue_init(Queue *queue, const char *name) {
    queue->size = 0;
    queue->head = nullptr;
    queue->tail = nullptr;
    queue->is_block = true;
    queue->name = name;
}

void queue_destroy(Queue *queue) {
    Node *tmp = queue->head;
    while (tmp != nullptr) {
        queue->head = queue->head->next;
        free(tmp);
        tmp = queue->head;
    }
    queue->head = nullptr;
    queue->tail = nullptr;
    queue->size = 0;
    queue->is_block = false;
}

bool queue_is_empty(Queue *queue) {
    if (queue->size == 0) {
        return true;
    }
    return false;
}

int queue_size(Queue *queue) {
    return queue->size;
}

bool queue_is_full(Queue *queue) {
    return queue->size == QUEUE_MAX_SIZE;
}

void queue_in(Queue *queue, NodeElement element) {
    std::unique_lock<std::mutex> lock(queue->mutex);
//    if (queue->name == "video") {
//        LOGI("queue_in size = %d", queue->size);
//    }
    queue->not_full_cv.wait(lock, [&](){
//        LOGI("queue_in size = %d, is_block = %d", queue->size, queue->is_block);
        return !queue_is_full(queue) || !queue->is_block;
    });
    Node *node = (Node *) malloc(sizeof(Node));
    node->data = element;
    node->next = nullptr;
    if (queue->head == nullptr) {
        queue->head = node;
        queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }
    queue->size = queue->size + 1;
    lock.unlock();
    queue->not_empty_cv.notify_one();
}

NodeElement queue_out(Queue *queue) {
    if (queue->head == nullptr) {
        return NULL;
    }
    std::unique_lock<std::mutex> lock(queue->mutex);
//    if (queue->name == "video") {
//        LOGI("queue_out size = %d", queue->size);
//    }
    queue->not_empty_cv.wait(lock, [&](){
//        LOGI("queue_out size = %d, is_block = %d", queue->size, queue->is_block);
        return !queue_is_empty(queue) || !queue->is_block;
    });
    Node *tmp = queue->head;
    queue->head = queue->head->next;
    if (queue->head == nullptr) {
        queue->tail = nullptr;
    }
    auto value = tmp->data;
    free(tmp);
    queue->size = queue->size - 1;
    lock.unlock();
    queue->not_full_cv.notify_one();
    return value;
}

void queue_clear(Queue *queue) {
    std::unique_lock<std::mutex> lock(queue->mutex);
    Node *node = queue->head;
    while (node != nullptr) {
        queue->head = queue->head->next;
        free(node);
        node = queue->head;
    }
    queue->head = nullptr;
    queue->tail = nullptr;
    queue->size = 0;
    queue->is_block = true;
    queue->not_full_cv.notify_one();
}

void break_block(Queue *queue) {
    queue->is_block = false;
    if (queue->name == "video") {
        LOGI("break_block size = %d", queue->size);
    }
    queue->not_empty_cv.notify_one();
    queue->not_full_cv.notify_one();
}
