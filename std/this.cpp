#include <iostream>
using namespace std;
class Test
{
public:
	int x;
	Test get()
	{
		return *this; //返回当前对象的拷贝
	}
};
int main()
{
	Test a;
	a.x = 4;
	if(a.x == a.get().x)
	{

        Test g = a.get();
		cout << a.x << endl;
                cout <<  &a << endl;
               cout << &g <<endl;
	}
	else
	{
        Test g = a.get();
		cout << "no" << endl;
                cout <<  &a << endl;
               // cout << &a.get() <<endl; //这个 error: taking address of rvalue [-fpermissive]
                 cout << &g <<endl; 
	}

	return 0;
}