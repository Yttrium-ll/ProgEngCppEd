#include <iostream>

class Base
{
public:

    virtual ~Base() = default;

    virtual void initialize() = 0;

}; // class Base

class Derived : public Base
{
protected: // good: ctor can not be called by user

    explicit Derived([[maybe_unused]] int x) {} 

public:

    void initialize() override { std::cout << "initialization" << std::endl; }

}; // class Derived : public Base

template < typename T > class Initializer : public T
{
public:

    template < typename ... Types > explicit Initializer(Types ... args) : T(args...)
    {
        this->initialize(); // good: mandatory function call
    }

}; // template < typename T > class Initializer : public T

int main()
{
    Initializer < Derived > derived(42);

    return 0;
}