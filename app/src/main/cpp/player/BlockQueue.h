//
// Created by okunu on 2023/9/28.
//

#ifndef OKPLAYER_BLOCKQUEUE_H
#define OKPLAYER_BLOCKQUEUE_H

#define MAX_SIZE 100

#include "LogUtil.h"
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

template<typename T>
class BlockQueue {

public:
    BlockQueue() : stop_(false) {
    }

    ~BlockQueue() {
        LOGI("BlockQueue delete %d", stop_);
        if (stop_) {
            pro_cv.notify_all();
            con_cv.notify_all();
        }
    }

    bool is_full() {
        return queue_.size() == MAX_SIZE;
    }

    bool is_empty() {
        return queue_.size() == 0;
    }

    int size() {
        return queue_.size();
    }

    void push(T &t) {
        if (stop_) {
            return;
        }
        unique_lock<mutex> lock(mutext_);
        while (is_full()) {
            con_cv.notify_one();
            pro_cv.wait(lock, [this]() { return !is_full() || stop_; });
        }
        if (stop_) {
            con_cv.notify_one();
            return;
        }
        queue_.emplace(t);
        con_cv.notify_one();
    }

    /**
     * 如果停止时，只有一种情况，即不能再生产了，同时把剩余的东西消费完毕后就退出了，不再等待了
     */
    T pop() {
        unique_lock<mutex> lock(mutext_);
        while (is_empty() && !stop_) {
            pro_cv.notify_one();
            con_cv.wait(lock, [this]() { return !is_empty() || stop_; });
        }
        if (is_empty() && stop_) {
            return NULL;
        }
        T t = queue_.front();
        queue_.pop();
        pro_cv.notify_one();
        return t;
    }

    void stop() {
        stop_ = true;
        pro_cv.notify_all();
        con_cv.notify_all();
    }

    void start() {
        stop_ = false;
        pro_cv.notify_all();
        con_cv.notify_all();
    }

private:
    std::queue<T> queue_;
    mutex mutext_;
    condition_variable pro_cv;
    condition_variable con_cv;
    bool stop_;
};

#endif //OKPLAYER_BLOCKQUEUE_H
