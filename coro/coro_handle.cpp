#include <iostream>
#include <coroutine>

struct Generator {
    struct promise_type {
        int current_value;
        auto get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};//获得 coroutine_handle
        }
        auto initial_suspend() { return std::suspend_always{}; }
        auto final_suspend() noexcept { return std::suspend_always{}; }
        void unhandled_exception() { std::terminate(); }
        void return_void() {}
        auto yield_value(int value) {
            current_value = value;
            return std::suspend_always{};
        }
    };
    std::coroutine_handle<promise_type> coro;
    explicit Generator(std::coroutine_handle<promise_type> h) : coro(h) {}
    ~Generator() { if (coro) coro.destroy(); }
    int get() {
        coro.resume();
        return coro.promise().current_value;
    }
};

Generator counter() {
    for (int i = 0; i < 5; ++i) {
        co_yield i;
    }
}

int main() {
    auto gen = counter();
    std::cout << gen.get() << std::endl;
    std::cout << gen.get() << std::endl;
    std::cout << gen.get() << std::endl;
    std::cout << gen.get() << std::endl;
    std::cout << gen.get() << std::endl;
    return 0;
}