#include <coroutine>
#include <iostream>
#include <thread>
// g++ -fcoroutines  --std=c++20 -lpthread thread.cpp
struct awaitable {
    std::jthread& t3;
    bool await_ready() { return false; }
    void await_suspend(std::coroutine_handle<> h) {
        std::jthread& out = t3;
        out = std::jthread([h] { h.resume(); });
        std::cout << "await_suspend [New thread] ID: " << out.get_id() << '\n';
    }
    void await_resume() {}
    ~awaitable() {
        std::cout << "Avaitable destroyed on [thread]: " << std::this_thread::get_id() << " with thread " << t3.get_id() << '\n';
    }
    awaitable(std::jthread& t) : t3(t) {
        std::cout << "Avaitable constructed on [thread]: " << std::this_thread::get_id() << '\n';
    }
};

struct task{
    struct promise_type {
        task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};

task coro(std::jthread& t1, std::jthread& t2, int i) {
    std::cout << "coro() Coroutine started on thread: " << std::this_thread::get_id() << " i=" << i << '\n';
    co_await awaitable{t1};
    std::cout << "coro() Coroutine resumed on [thread]: " << std::this_thread::get_id() << " i=" << i << '\n';
    
    // #if 0
    // std::cout << "coro() Coroutine continues on thread: " << std::this_thread::get_id() << " i=" << i << '\n';    
    // co_await awaitable{t2};
    // // awaiter destroyed here
    // std::cout << "coro() Coroutine resumed on thread: " << std::this_thread::get_id() << " i=" << i << '\n';
    // #endif

    std::cout << "coro() Coroutine done on thread: " << std::this_thread::get_id() << " i=" << i << '\n';
}

int main() {
    std::cout << "main(): " << std::this_thread::get_id() << '\n';
    {
        std::jthread t1, t2;
        coro(t1, t2, 42);
        std::cout << "main(): " << std::this_thread::get_id() << " thread done" << std::endl;
    }
    std::cout << "main(): " << std::this_thread::get_id() <<" really done"<< std::endl;
}