//c++17的折叠表达式
#include <bits/stdc++.h>
using namespace std;
template<typename... Args>
int sum(Args... args) {
    return (args + ...);
}

int main() {
    std::cout << sum(1, 2, 3, 4, 5) << '\n';  // 输出15
    return 0;
}
