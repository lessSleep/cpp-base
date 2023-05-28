#include <iostream>
#include <chrono>
#include <functional>

auto res = 0;
#pragma GCC push_options
#pragma GCC optimize ("O0")
long test_func(long i)
{
	return i;
}
#pragma GCC pop_options

auto test_lambda = [](int i)
{
	test_func(i);
};


auto test_bind = std::bind(test_func, std::placeholders::_1);

auto test_func_ptr = test_func;
int main()
{
	//auto times = 10000000000LL;
    auto times = 1000000000LL;
	// warm cache

	for (auto i = 0LL; i < times; i++)
		test_func(i);

	auto t0 = std::chrono::high_resolution_clock::now();
	for (auto i = 0LL; i < times; i++)
		test_lambda(i);
	auto t1 = std::chrono::high_resolution_clock::now();

	auto t2 = std::chrono::high_resolution_clock::now();
	for (auto i = 0LL; i < times; i++)
		test_bind(i);
	auto t3 = std::chrono::high_resolution_clock::now();

	auto t4 = std::chrono::high_resolution_clock::now();
	for (auto i = 0LL; i < times; i++)
		test_func_ptr(i);
	auto t5 = std::chrono::high_resolution_clock::now();

	std::cout << "lambda: " << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << std::endl;
	std::cout << "bind: " << std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count() << std::endl;
	std::cout << "func_ptr: " << std::chrono::duration_cast<std::chrono::milliseconds>(t5 - t4).count() << std::endl;

	//system("pause");
	return 0;
}