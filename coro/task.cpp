#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <coroutine>

class ThreadPool {
public:
    ThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; ++i) {
            threads_.emplace_back([this] {
                while (true) {
                    std::unique_lock<std::mutex> lock(mutex_);
                    condition_.wait(lock, [this] { return !tasks_.empty(); });
                    auto task = std::move(tasks_.front());
                    tasks_.pop();
                    lock.unlock();
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            stop_ = true;
        }
        condition_.notify_all();
        for (auto& thread : threads_) {
            thread.join();
        }
    }

    template <typename Func>
    auto enqueue(Func&& func) {
        using ReturnT = std::invoke_result_t<Func>;
        using PromiseT = std::promise<ReturnT>;
        using FutureT = std::future<ReturnT>;

        auto task = [promise = PromiseT{}(), func = std::forward<Func>(func)]() mutable {
            try {
                promise.set_value(func());
            } catch (...) {
                promise.set_exception(std::current_exception());
            }
        };

        auto future = task.get_return_object();
        {
            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.emplace(std::move(task));
        }
        condition_.notify_one();
        return future;
    }

private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    //std::queue<std::function<void()>> tasks_;
    std::queue<std::coroutine_handle<>> tasks_;

    std::mutex mutex_;
    std::condition_variable condition_;
    bool stop_ = false;
};

int main() {
    ThreadPool pool{4};
    std::vector<std::future<int>> futures;
    for (int i = 0; i < 8; ++i) {
        futures.emplace_back(pool.enqueue([i] {
            std::cout << "Task " << i << " started\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "Task " << i << " finished\n";
            return i * i;
        }));
    }
    for (auto& future : futures) {
        std::cout << "Result: " << future.get() << '\n';
    }    
    return 0;
}
