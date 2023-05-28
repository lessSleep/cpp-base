#include <iostream>
struct abc {
    int a = 0;
    int b = 0;
};

void fun(const abc& x) {
    std::cout << x.a << "\n"; // x.a 比下面的  x->a 少写了一个字符
}
void fun(const abc* x) {
    std::cout << x->a << "\n"; 
}

int main(){
    abc val = {2,3};
    fun(val); // 调用 fun(const abc& x) 
    fun(&val); // 调用 fun(const abc* x)
    return 0;
}