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

// 作者：鲁班代师
// 链接：https://juejin.cn/post/7204786218501013541
// 来源：稀土掘金
// 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。