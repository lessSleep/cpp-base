#include <iostream>
using namespace std;

class A
{
public:
	int x;
	
	//返回当前对象本身
	A& get()
	{
		return *this;
	}

	//返回当前对象的克隆
	A get_a()
	{
		return *this; //返回当前对象的拷贝
	}
	
	//返回指向当前对象的指针,即当前对象的地址

	A* get_b() {
		return this;
	}
	
};

int main()
{
	A a;
	a.x = 4;

	//test&(){return *this}
	if (&a == &a.get())
	{
		cout << "yes" << endl;
	}
	else
	{
		cout << "no" << endl;
	}

	//test(){return *this}
	// if (&a == &a.get_a())
	// {
	// 	cout << "yes" << endl;
	// }
	// else
	// {
	// 	cout << "no" << endl;
	// }

	//test* (){retun this};
	if (&a == a.get_b())
	{
		//cout << a.get_b() << endl;
		cout << "yes" << endl;
	}
	else
	{
		cout << "no" << endl;
	}

	return 0;
}
