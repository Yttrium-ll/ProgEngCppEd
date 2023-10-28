#include <iostream>
#include <string>

class Shape 
{
public:

    virtual ~Shape() = default;

public:

    virtual std::string description() const = 0;

}; // class Shape 

class Polygon : public Shape 
{
public:

    std::string description() const override 
    { 
        return "Polygon"; 
    }

}; // class Polygon

class Decorator : public Shape // note: abstract decorator
{
public:

    explicit Decorator(Shape * shape): m_shape(shape) 
    {
        if (!m_shape) // good: verify if nullptr
        {
            std::cout << "invalid shape" << std::endl;
        }
    }

public:

    auto shape() const { return m_shape; }

    std::string description() const override 
    { 
        return m_shape->description(); 
    }

private:

    Shape * m_shape;

}; // class Decorator : public Shape 

class Colored_Shape : public Decorator // note: concrete decorator
{
public:

    explicit Colored_Shape(Shape * shape) : Decorator(shape) {}

public:

    std::string description() const override 
    { 
        return shape()->description() + " with Color"; 
    }

}; // class Colored_Shape : public Decorator

int main()
{
    Shape * shape = new Polygon();

    std::cout << shape->description() << std::endl;

    Shape * colored_shape = new Colored_Shape(shape);

    std::cout << colored_shape->description() << std::endl;

    delete shape; // good: no memory leak

    delete colored_shape; // good: no memory leak

    return 0;
}