#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#include <boost/flyweight.hpp>

//  ================================================================================================

struct Entity
{
    using data_1_t = int;

    using data_2_t = std::string;

//  ------------------------------------------------------------------------------------------------

    template < typename T > using tag_t = boost::flyweights::tag < T > ;

    struct data_1_tag {};
    struct data_2_tag {};

//  ------------------------------------------------------------------------------------------------

    explicit Entity(data_1_t data_1, const data_2_t & data_2) 
    : 
        flyweight_data_1(data_1), 
        flyweight_data_2(data_2) 
    {}

//  ------------------------------------------------------------------------------------------------

    boost::flyweight < data_1_t, tag_t < data_1_tag > > flyweight_data_1;
    boost::flyweight < data_2_t, tag_t < data_2_tag > > flyweight_data_2;
};

//  ================================================================================================

int main() 
{
    auto size_1 = 1'000'000uz, size_2 = 1'000uz;

    std::vector < Entity > entities; 
    
    entities.reserve(size_1);

    for (auto i = 0uz; i < size_1; ++i)
    {
        entities.emplace_back(1, std::string(size_2, 'a'));
    }

    std::cout << "Enter any character to continue : "; char c; std::cin >> c;

//  ------------------------------------------------------------------------------------------------

    auto & entity = entities.front();

    for (auto i = 1uz; i < size_1; ++i)
    {
        assert(&entity.flyweight_data_1.get() == &entities[i].flyweight_data_1.get());
        assert(&entity.flyweight_data_2.get() == &entities[i].flyweight_data_2.get());
    }

    entity = Entity(2, std::string(size_2, 'b'));

    for (auto i = 1uz; i < size_1; ++i)
    {
        assert(&entity.flyweight_data_1.get() != &entities[i].flyweight_data_1.get());
        assert(&entity.flyweight_data_2.get() != &entities[i].flyweight_data_2.get());
    }
}