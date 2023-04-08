// #include <mutex>
// #include <condition_variable>
// #include <functional>
// #include <queue>
// #include <thread>
//https://stackoverflow.com/questions/46564845/perfect-forwarding-of-references-with-stdbind-inside-variadic-template
#include <bits/stdc++.h>
using namespace std;

template <class T> std::reference_wrapper<T> maybe_wrap(T& val) { return std::ref(val); }
template <class T> T&& maybe_wrap(T&& val) { return std::forward<T>(val); }

template<typename F, typename ... Args>
auto eval(F&& _f, Args&&... _args)
{
    //这样写编译会报错
    //using ret_t = typename std::result_of<F&(Args&...)>::type;
    //std::function<ret_t()> func(std::bind(std::forward<F>(_f), std::forward<Args>(_args)...));

    using ret_t = typename std::result_of<F&&(Args&&...)>::type;
    std::function<ret_t()> func(std::bind(std::forward<F>(_f), maybe_wrap(std::forward<Args>(_args))...)); 


    return func;
    // Etc.
}

void f(int a){
    cout<<"result:"<<a<<endl;
}

class NoCopy {
public:
    NoCopy(int x) : data(x) {} // constructor with int parameter
    NoCopy(const NoCopy&) = delete; // disable copy constructor
    NoCopy& operator=(const NoCopy&) = delete; // disable copy assignment operator
    // rest of the class implementation
    int operator()() const { return data; } // overload of the function call operator
private:
    int data;
};

// Example usage:
// NoCopy a(5); // constructor with int parameter
// NoCopy b(a); // error: copy constructor is deleted
// NoCopy c = a; // error: copy constructor is deleted

void fncp(NoCopy& a) {
    cout<<"result:"<<a()<<endl;
}

int main() {
   auto a = eval(f,1);
   //eval(f,1)();
   a();

   NoCopy b(5);
   auto ncp = eval(fncp,b);
   ncp();
}