#include <iostream>
#include <vector>

class Computer // note: polymorphic base class
{
public:

	Computer()
	{
//		print(); // bad: base version
	}

	virtual ~Computer()
	{
//		print(); // bad: base version
	}

public:

	virtual void print() const // note: can be redefined in derived classes
	{
		std::cout << "Computer" << std::endl;
	}

}; // class Computer

class Mobile final : public Computer // note: final class in hierarchy
{
public:

	void print() const override final // note: final function in hierarchy
	{
		std::cout << "Mobile" << std::endl;
	}

}; // class Mobile final : public Computer

/*
class Tablet : public Mobile // error: class Mobile declared final
{
public:

	void print() const override // error: print declared final in class Mobile
	{
		std::cout << "Tablet" << std::endl;
	}

}; // class Tablet : public Mobile
*/

class Laptop : public Computer // note: not final class in hierarchy
{
public:

	void print() const override // note: not final function in hierarchy
	{
		std::cout << "Laptop" << std::endl;
	}

}; // class Laptop : public Computer

class Abstract_Base // note: interface class like in Java
{
public:

	virtual ~Abstract_Base() = default; // note: polymorphic base class

public:

	virtual void print() const = 0; // note: must be redefined in derived classes

}; // class Abstract_Base

void Abstract_Base::print() const // note: definition must be provided separately
{
	std::cout << "default implementation" << std::endl;
}

class Derived : public Abstract_Base
{
public:

	void print() const override
	{
		std::cout << "derived implementation" << std::endl;

		Abstract_Base::print(); // note: possible default implementation
	}

}; // class Derived : public Abstract_Base

int main()
{
	Mobile mobile;

	Computer * c_ptr = &mobile; // note: works with pointers and references

//	Computer s = p; // bad: object slicing

	c_ptr->print();

	Mobile mobile_1;
	Mobile mobile_2;
	Laptop laptop_1;
	Laptop laptop_2;

	std::vector < Computer * > computers = { &mobile_1, &mobile_2, &laptop_1, &laptop_2 };

	for (std::size_t i = 0; i < computers.size(); ++i)
	{
		computers[i]->print();
	}

	c_ptr = new Mobile;

	c_ptr->print();

	delete c_ptr; // note: important virtual destructor

	Derived d;

	Abstract_Base * ab_ptr = &d; // good: interface class

	ab_ptr->print();

//	Abstract_Base ab; // error: abstract base class

	return 0;
}