//
// Created by okunu on 2023/9/23.
//

#ifndef OKPLAYER_THREADPOOL_H
#define OKPLAYER_THREADPOOL_H

#include <thread>
#include <mutex>
#include <functional>
#include <memory>
#include <queue>
#include <vector>
#include <future>
#include "LogUtil.h"

using namespace std;

class ThreadPool {
public:
    using Task = std::function<void()>;

    ThreadPool(int num): thread_num_(num), run_(true){
        addThread();
    }

    ~ThreadPool(){
        run_ = false;
        cv.notify_all();
        for (auto& item:thread_list_) {
            if (item.joinable()) {
                item.join();
            }
        }
    }

    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args)-> future<decltype(f(args...))> {
        if (!run_) {
            LOGI("thread pool shutdown");
        }
        using RetType = decltype(f(args...));
        auto task = make_shared<packaged_task<RetType()>>(bind(forward<F>(f), forward<Args>(args)...));
        future<RetType> tmp = task->get_future();
        {
            lock_guard<mutex> lock(mutex_);
            queue_.emplace([task](){
                (*task)();
            });
        }
        cv.notify_one();
        return tmp;
    }


private:
    void addThread() {
        for (int i = 0; i < thread_num_; ++i) {
            thread_list_.emplace_back(thread([this](){
                while (run_) {
                    Task task;
                    {
                        unique_lock<mutex> lock(mutex_);
                        cv.wait(lock, [this](){
                            return !run_ || !queue_.empty();
                        });
                        LOGI("thread pool size = %d", queue_.size());
                        if (queue_.empty()) {
                            break;
                        }
                        task = move(queue_.front());
                        queue_.pop();
                    }
                    task();
                }
            }));
        }
    }

private:
    std::queue<Task> queue_;
    vector<thread> thread_list_;
    int thread_num_;
    bool run_;
    mutex mutex_;
    condition_variable cv;
};

#endif //OKPLAYER_THREADPOOL_H
