#include <cassert>
#include <iostream>
#include <vector>

//  ================================================================================================

class Vector 
{
public:

    explicit Vector(std::size_t size) : m_data(size, 0) {}

//  ------------------------------------------------------------------------------------------------

    const auto & operator[](std::size_t index) const 
    {
        std::clog << "Vector::operator[] (1)\n";

        return m_data[index];
    }

    auto & operator[](std::size_t index)
    {
        std::clog << "Vector::operator[] (2)\n";

        return const_cast < int & > (static_cast < const Vector & > (*this)[index]);
    }

private:

    std::vector < int > m_data;
};

//  ================================================================================================

int main()
{
    const auto x = 1;

    const auto & r_x = x;

    const_cast < int & > (r_x) = 2; 
    
    assert(x == 2);

//  ------------------------------------------------------------------------------------------------

    assert(Vector(5)[0] == 0);
}