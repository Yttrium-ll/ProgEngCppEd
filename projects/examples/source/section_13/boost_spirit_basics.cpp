#include <cassert>
#include <iostream>
#include <iterator>
#include <string>
#include <tuple>
#include <utility>

using namespace std::literals;

#include <boost/fusion/include/std_tuple.hpp>
#include <boost/fusion/include/std_pair.hpp>

#include <boost/spirit/home/x3.hpp>

int main()
{
    auto value = 0; 
    
    auto input_1 = "42"s;

    boost::spirit::x3::parse(std::begin(input_1), std::end(input_1), boost::spirit::x3::int_, value);

    assert(value == 42);

    auto input_2 = "42 42"s;

    std::pair < int, int > pair;

    boost::spirit::x3::phrase_parse(std::begin(input_2), std::end(input_2), 
        boost::spirit::x3::int_ >> 
        boost::spirit::x3::int_, 
        boost::spirit::x3::space, pair); // note: skip space characters

    assert(pair.first  == 42);
    assert(pair.second == 42);

    auto input_3 = "(42, 42)"s;

    std::tuple < int, int > tuple;

    boost::spirit::x3::phrase_parse(std::begin(input_3), std::end(input_3), '(' >> 
        boost::spirit::x3::int_ >> ',' >> 
        boost::spirit::x3::int_ >> ')', 
        boost::spirit::x3::space, tuple); // note: skip space characters

    assert(std::get < 0 > (tuple) == 42);
    assert(std::get < 1 > (tuple) == 42);

    auto input_4 = "42"s;

    auto f = [](auto & context)
    { 
        std::cout << boost::spirit::x3::_attr(context) << std::endl; 
    };

    boost::spirit::x3::parse(std::begin(input_4), std::end(input_4), boost::spirit::x3::int_[f]);

    return 0;
}