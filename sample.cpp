#include "zlinq.hpp"
#include <vector>
#include <string>
#include <iostream>

using namespace zlinq;

template<typename Function>
void run_test(const Function& func)
{
	std::cout << "=====================" << std::endl;
	func();
	std::cout << "=====================" << std::endl;
}

void test1()
{
	typedef std::pair<int, std::string> inner_type;
	std::vector<inner_type> a;
	a.push_back(std::make_pair(0, "a"));
	a.push_back(std::make_pair(1, "b"));
	a.push_back(std::make_pair(2, "c"));
	a.push_back(std::make_pair(3, "d"));
	a.push_back(std::make_pair(4, "f"));
	a.push_back(std::make_pair(4, "g"));

	auto res = From(a)
		.Where([](inner_type i) { return i.first > 1; })
		.Select([](inner_type i) { return i.second; });
	for (auto it = res.begin(); it != res.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
}

void test2()
{
	int a[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
	auto res = From(a).Where([](int i){return i > 5;});
	for (auto it = res.begin(); it != res.end(); ++it)
	{
		std::cout << *it << std::endl;
	}
	std::cout << "all > 8 " << res.All([](int i){return i > 8;}) << std::endl;
	std::cout << "any > 8 " << res.Any([](int i){return i > 8;}) << std::endl;
	std::cout << "count > 8 " << res.Count([](int i){return i > 8;}) << std::endl;
	std::cout << "count " << res.Count() << std::endl;
	std::cout << "sum " << res.Sum() << std::endl;
}

int main()
{
	run_test(test1);
	run_test(test2);
	return 0;
}
