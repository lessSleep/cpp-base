#include <iostream>

using namespace std;

class Test
{
public:
    int m_i;
    double m_j;

public:
    Test(int i = 0, double j = 0.0) : m_i(i), m_j(j)
    {
        cout << "constructor called " << endl;
    }
    Test &operator=(Test &rhs)
    {
        cout << "copy assignment called " << endl;
        this->m_i = rhs.m_i;
        this->m_j = rhs.m_j;
        return *this;
    }
};

int main()
{
    Test obj(1, 2.0);
    cout << obj.m_i << " - " << obj.m_j << endl;
    Test obj2;
    Test obj3;
    cout << "before obj3 = obj2 = obj;" << endl;    
    obj3 = obj2 = obj; // 这里调用obj2的=运算符，这个运算返回一个obj&。再传给obj3的= 运算符。
    cout << obj2.m_i << " - " << obj2.m_j << endl;    
    cout << obj3.m_i << " - " << obj3.m_j << endl;


    return 0;
}


// C++ return * this的理解
// 在c++代码中自定义操作符的时候，例如前置++, = 赋值操作的时候会返回*this。this是类自带的指针，指代的是对象本身。*this对指针解引用，因此使用*this返回的就是一个对象本身。这样的操作可以满足连续的= (赋值操作)。试想一下如果返回的是this,那么返回的是指针。
// ————————————————
// 版权声明：本文为CSDN博主「anuowu」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
// 原文链接：https://blog.csdn.net/anuowu/article/details/121065576