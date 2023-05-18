#include <iostream>
#include <tuple>

using namespace std;

template <typename Tuple, typename Func, size_t ... N>
void func_call_tuple(const Tuple& t, Func&& func, std::index_sequence<N...>) {
    static_cast<void>(std::initializer_list<int>{(func(std::get<N>(t)), 0)...});
}

template <typename ... Args, typename Func>
void travel_tuple(const std::tuple<Args...>& t, Func&& func) {
    func_call_tuple(t, std::forward<Func>(func), std::make_index_sequence<sizeof...(Args)>{});
}


//https://www.fluentcpp.com/2021/03/05/stdindex_sequence-and-its-improvement-in-c20/

int f0()
{
    auto lambda = [] <typename T>(T value){ return value + 1; };
    return lambda(40);
}

int f()
{
    return  [] <typename T>(T value){ return value + 1; }(41);
    // return lambda(41);
}

template <class Tuple, class F>
constexpr decltype(auto) for_each(Tuple&& tuple, F&& f)
{
    return [] <std::size_t... I>
    (Tuple&& tuple, F&& f, std::index_sequence<I...>)
    {
        (f(std::get<I>(tuple)), ...);
        return f;
    } (std::forward<Tuple>(tuple), std::forward<F>(f), std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

int main() {
    auto t = std::make_tuple(1, 4.56, "happen lee");
    travel_tuple(t, [](auto&& item) {
        std::cout << item << ",";
    });

    std::cout <<  f0() <<endl;
    std::cout <<  f() <<endl;
}