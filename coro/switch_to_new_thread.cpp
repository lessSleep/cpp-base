#include <coroutine>
#include <iostream>
#include <stdexcept>
#include <thread>
//https://zh.cppreference.com/w/cpp/language/coroutines
auto switch_to_new_thread(std::jthread& out)
{
    struct awaitable
    {
        std::jthread* p_out;
        bool await_ready() { return false; } //这里返回 false，就会先执行await_suspend，之后交出控制权
        void await_suspend(std::coroutine_handle<> h)
        {
            std::jthread& out = *p_out;
            if (out.joinable())
                throw std::runtime_error("jthread 输出参数非空");
            out = std::jthread([h] { h.resume(); });
            // 潜在的未定义行为：访问潜在被销毁的 *this
            // std::cout << "新线程 ID：" << p_out->get_id() << '\n';
            std::cout << "新线程 ID：" << out.get_id() << '\n'; // 这样没问题
        }
        void await_resume() {}
    };
    return awaitable{&out};
}
 
struct task
{
    struct promise_type
    {
        task get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() {}
    };
};
 
task resuming_on_new_thread(std::jthread& out)
{
    std::cout << "协程开始，线程 ID：" << std::this_thread::get_id() << '\n';
    co_await switch_to_new_thread(out);
    // 等待器在此销毁
    std::cout << "协程恢复，线程 ID：" << std::this_thread::get_id() << '\n';
}
 
int main()
{
    std::jthread out;
    resuming_on_new_thread(out);
}

// ubuntu2004@DESKTOP-OEAU8BF:~/code/cpp-base/coro$ g++ -fcoroutines --std=c++20 -lpthread switch_to_new_thread.cpp
// ubuntu2004@DESKTOP-OEAU8BF:~/code/cpp-base/coro$ ./a.out
// 协程开始，线程 ID：140075347412800
// 新线程 ID：140075347408640
// 协程恢复，线程 ID：140075347408640