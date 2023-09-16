#include <iostream>

int main()
{
	int x = 0;
	int * ptr_x = &x;

	auto y = 0;
	auto ptr_y = &y;

	*ptr_x = 42;
	++(*ptr_y);

	std::cout << ptr_x << ' ' << *ptr_x << '\n';
	std::cout << ptr_y << ' ' << *ptr_y << '\n';

	int * ptr_z = nullptr; // good: nullptr instead of 0

	if (ptr_z) // good: check pointer before dereferencing
	{
		std::cout << *ptr_z << '\n';
	}

	auto c = 0;

	const int * ptr_c = &c;

//	*ptr_c = 42; // error: constant value

	int * const c_ptr = &c;

//	c_ptr = nullptr; // error: constant pointer

	const int * const c_ptr_c = &c;

	return 0;
}