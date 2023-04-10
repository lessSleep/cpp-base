// This code demonstrates the use of await_transform in C++20
// It requires a compiler that supports C++20 features such as coroutines and concepts

#include <iostream>
#include <coroutine>
#include <chrono>
#include <thread>
#include <bits/stdc++.h>
// Define a coroutine that waits for a specified duration
class wait_for_seconds {
public:
    std::chrono::seconds duration_;
    wait_for_seconds(std::chrono::seconds duration) : duration_(duration) {}

    // The coroutine promise type
    struct promise_type {
        wait_for_seconds get_return_object() { return wait_for_seconds{std::chrono::seconds{0}}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };

    // The coroutine handle type
    struct awaiter {
        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<> handle) const {
            std::thread([handle, duration = duration_] {
                std::this_thread::sleep_for(duration);
                handle.resume();
            }).detach();
        }
        void await_resume() const noexcept {}
    };

    // The awaitable type
    auto operator co_await() const noexcept { return awaiter{}; }

//private:
    
};

// Define a coroutine that waits for a specified number of iterations
class wait_for_iterations {
public:
    wait_for_iterations(int iterations) : iterations_(iterations) {}

    // The coroutine promise type
    struct promise_type {
        wait_for_iterations get_return_object() { return wait_for_iterations{0}; }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };

    // The coroutine handle type
    struct awaiter {
        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<promise_type> handle) const {
            if (++handle.promise().count_ < handle.promise().iterations_) {
                handle.resume();
            } else {
                handle.promise().count_ = 0;
                handle.promise().coroutine_.resume();
            }
        }
        void await_resume() const noexcept {}
    };

    // The awaitable type
    auto operator co_await() const noexcept { return awaiter{}; }

private:
    int iterations_;
};

// Define a coroutine that uses await_transform to wait for a specified duration and number of iterations
std::coroutine_handle<> coroutine_handle;

struct coroutine_awaiter {
    bool await_ready() const noexcept { return false; }
    void await_suspend(std::coroutine_handle<> handle) const {
        coroutine_handle = handle;
    }
    void await_resume() const noexcept {}
};

class wait_for_seconds_and_iterations {
public:
    wait_for_seconds_and_iterations(std::chrono::seconds duration, int iterations) : duration_(duration), iterations_(iterations) {}

    // The coroutine promise type
    struct promise_type {
        wait_for_seconds_and_iterations get_return_object() { return wait_for_seconds_and_iterations{std::chrono::seconds{0}, 0}; }
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
        auto await_transform(coroutine_awaiter) { return std::suspend_always{}; }
    };

    // The coroutine handle type
    struct awaiter {
        bool await_ready() const noexcept { return false; }
        void await_suspend(std::coroutine_handle<promise_type> handle) const {
            handle.promise().coroutine_ = coroutine_handle;
            handle.promise().duration_ = duration_;
            handle.promise().iterations_ = iterations_;
            handle.promise().count_ = 0;
            handle.resume();
        }
        void await_resume() const noexcept {}
    };

    // The awaitable type
    auto operator co_await() const noexcept { return awaiter{}; }

private:
    std::chrono::seconds duration_;
    int iterations_;
    int count_;
    std::coroutine_handle<> coroutine_;
};

// The main function that uses the coroutines
int main() {
    std::cout << "Starting coroutine...\n";
    auto start_time = std::chrono::steady_clock::now();
    co_await wait_for_seconds_and_iterations{std::chrono::seconds{2}, 3};
    auto end_time = std::chrono::steady_clock::now();
    std::cout << "Coroutine finished after " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() << "ms\n";
    return 0;
}