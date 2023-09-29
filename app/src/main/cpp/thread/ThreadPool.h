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
#include <chrono>

using namespace std;

class ThreadPool {
public:
    using Task = std::function<void()>;

    ThreadPool(int num) : thread_num_(num), run_(true) {
        addThread();
    }

    ~ThreadPool() {
        run_ = false;
        cv.notify_all();
        for (auto &item: thread_list_) {
            if (item.joinable()) {
                item.join();
            }
        }
    }

    template<typename F, typename... Args>
    auto submit(const char* flag, F &&f, Args &&... args) -> future<decltype(f(args...))> {
        LOGI("submit task %s", flag);
        auto start = std::chrono::high_resolution_clock::now();
        if (!run_) {
            LOGI("thread pool shutdown");
        }
        using RetType = decltype(f(args...));
        auto task = make_shared<packaged_task<RetType()>>(
                bind(forward<F>(f), forward<Args>(args)...));
        future<RetType> tmp = task->get_future();
        {
            unique_lock<mutex> lock(mutex_);
            queue_.emplace([task]() {
                (*task)();
            });
            cv.notify_one();
        }
        auto end = std::chrono::high_resolution_clock::now();  // 记录结束时间
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);  // 计算执行时间
        LOGI("submit end %s, duration = %d", flag, duration.count());
        return tmp;
    }


private:
    void addThread() {
        for (int i = 0; i < thread_num_; ++i) {
            thread_list_.emplace_back(thread([this, i]() {
                while (run_) {
                    LOGI("current thread is %d", i);
                    Task task;
                    {
                        unique_lock<mutex> lock(mutex_);
                        cv.wait(lock, [this]() {
                            return !run_ || !queue_.empty();
                        });
                        LOGI("thread pool size = %zu , current thread is %d", queue_.size(), i);
                        if (queue_.empty() && !run_) {
                            break;
                        }
                        task = std::move(queue_.front());
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
    bool run_ = true;
    mutex mutex_;
    condition_variable cv;
};

#endif //OKPLAYER_THREADPOOL_H
