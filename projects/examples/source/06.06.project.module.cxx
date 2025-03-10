module;

#define DEBUG(string) std::cout << __func__ << " : " << string << '\n'

export module demo;

export import demo.submodule;

import <iostream>;

export namespace demo
{
	void test_v1() { std::cout << "demo::test_v1\n"; }

	void test_v2();
}

namespace demo
{
	void test_v3() 
	{ 
		std::cout << "demo::test_v3\n";
	}
}