//https://godbolt.org/z/1sb46aW5P
//https://codereview.stackexchange.com/questions/284282/c-thread-pool-with-suspend-functionality
//g++ -Wall -Wextra -lpthread -std=c++20 p1.cpp
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <cassert>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <queue>
#include <optional>
#include <sstream>
template <class T>
struct ThreadsafeQueue
{
    std::optional<T> pop()
    {
        auto l = lock();
        cv.wait(l, [&]
                { return !q.empty() || aborted; });
        if (aborted)
        {
            return std::nullopt;
        }
        auto front = std::move(q.front());
        q.pop_front();
        return front;
    }

    void push(T value)
    {
        auto l = lock();
        q.push_back(std::move(value));
        cv.notify_one();
    }

    void push_front(T value)
    {
        auto l = lock();
        q.push_front(std::move(value));
        cv.notify_one();
    }

    void abort()
    {
        auto l = lock();
        aborted = true;
        q.clear();
        cv.notify_all();
    }

private:
    auto lock() const
    {
        return std::unique_lock(m);
    }

    mutable std::mutex m;
    mutable std::condition_variable cv;
    std::deque<T> q;
    bool aborted = false;
};

using TaskQueue = ThreadsafeQueue<std::function<void()>>;

struct ThreadPool
{
    explicit ThreadPool(std::size_t thread_count)
        : threads(thread_count)
    {
        for (auto &thread : threads)
        {
            thread = std::jthread(task_worker, this);
        }
    }

    void abort()
    {
        queue.abort();
    }

    void add_task(std::function<void()> task)
    {
        queue.push(std::move(task));
    }

    TaskQueue &get_queue()
    {
        return queue;
    }

    std::size_t thread_count()
    {
        return threads.size();
    }

private:
    TaskQueue queue;
    std::vector<std::jthread> threads;

    static void task_worker(ThreadPool *self)
    {
        while (auto task = self->queue.pop())
        {
            (*task)();
        }
    }
};

struct Gate
{
    void open_gate()
    {
        auto l = lock();
        is_open = true;
        cv.notify_all();
    }

    void wait() const
    {
        auto l = lock();
        cv.wait(l, [&]
                { return is_open; });
    }

private:
    std::unique_lock<std::mutex> lock() const
    {
        return std::unique_lock<std::mutex>(m);
    }

    mutable std::mutex m;
    mutable std::condition_variable cv;
    bool is_open = false;
};

struct Semaphore
{
    explicit Semaphore(std::size_t count)
        : counter(count)
    {
    }

    void decrement(std::size_t n)
    {
        auto l = lock();
        if (counter < n) counter = 0;
        else counter -= n;
        cv.notify_all();
    }

    void wait_on_zero() const
    {
        auto l = lock();
        cv.wait(l, [&] { return counter == 0; });
    }

private:
    std::unique_lock<std::mutex> lock() const
    {
        return std::unique_lock<std::mutex>(m);
    }

    mutable std::mutex m;
    mutable std::condition_variable cv;
    std::size_t counter;
};

struct Suspender
{
    Suspender(ThreadPool& pool)
        : state(std::make_shared<helper>(pool.thread_count()))
    {
        for (std::size_t i = 0; i < pool.thread_count(); ++i)
        {
            pool.get_queue().push_front(
              [state=state]{
                thread_suspender(state);
              }
            );
        }
        state->sem.wait_on_zero();
    }

    ~Suspender()
    {
        state->gate.open_gate();
    }

    Suspender(Suspender const&)=delete;
    Suspender(Suspender&&)=default;
    Suspender& operator=(Suspender const&)=delete;
    Suspender& operator=(Suspender&&)=default;
private:
    struct helper
    {
        Semaphore sem;
        Gate gate;
        explicit helper(std::size_t count):sem(count){}
    };

    std::shared_ptr<helper> state;

    static void thread_suspender(std::shared_ptr<helper> state)
    {
        state->sem.decrement(1);
        state->gate.wait();
    }
};

int main()
{
    ThreadPool pool(4);

    constexpr std::size_t num_tasks = 100;
    constexpr std::size_t num_suspender_threads = 2;

    std::atomic<int> counter(0);

    std::vector<std::jthread> suspender_threads(num_suspender_threads);

    for (auto &thread : suspender_threads)
    {
        thread = std::jthread(
          [&] { 
            Suspender suspender(pool); 
            for (std::size_t i = 0; i < num_tasks / num_suspender_threads; ++i)
            {
              ++counter; 
            }
          }
        );
    }
    for (std::size_t i = 0; i < num_tasks; ++i)
    {
        pool.add_task([&]
                      { ++counter; });
    }


    for (auto &thread : suspender_threads)
    {
        thread.join();
    }

    pool.abort();

    assert(counter == (num_tasks + num_tasks));

    // using namespace std;
    // cout << counter << endl;

    return 0;
}