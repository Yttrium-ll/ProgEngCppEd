#include <iomanip>
#include <iostream>
#include <iterator>
#include <typeinfo>
#include <vector>

#include <boost/multi_array.hpp>

//  ================================================================================================

template < std::size_t D, typename C, typename I > void fill_shape(const C & container, I shape)
{
	if constexpr (*shape = std::size(container); D > 1)
	{
		fill_shape < D - 1 > (*(std::begin(container)), ++shape);
	}
}

//  ================================================================================================

template < std::size_t D, typename C, typename I > void fill_array(const C & container, I array)
{
	if constexpr (D > 1)
	{
		for (const auto & element : container) 
		{
			fill_array < D - 1 > (element, (array++)->begin());
		}
	}
	else
	{
		for (const auto & element : container) 
		{
			*(array++) = element;
		}
	}
}

//  ================================================================================================

template < typename T, std::size_t D, typename C > auto make_array(const C & container)
{
	std::vector < typename boost::multi_array < T, D > ::index > shape(D, 0);

	fill_shape < D > (container, std::begin(shape)); 
	
	boost::multi_array < T, D > array(shape);
	
	fill_array < D > (container, std::begin(array));

	return array;
}

//  ================================================================================================

int main()
{
	auto size_1 = 3uz;
	auto size_2 = 4uz;
	auto size_3 = 5uz;

//  ================================================================================================

	std::vector < std::vector < std::vector < int > > > vector_3D(size_1,
		          std::vector < std::vector < int > >            (size_2,
			                    std::vector < int >              (size_3, 0)));

	std::cout << "std::vector < std::vector < std::vector < int > > > " << std::endl << std::endl;

	auto counter = 0;

	for (auto i = 0uz; i < size_1; ++i)
	{
		for (auto j = 0uz; j < size_2; ++j)
		{
			for (auto k = 0uz; k < size_3; ++k)
			{
				std::cout << std::setw(2) << std::setfill(' ') << std::right;
				
				std::cout << (vector_3D[i][j][k] = ++counter) << " ";
			}

			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

//  ================================================================================================

	auto array = make_array < int, 3 > (vector_3D);

	std::cout << typeid(array).name() << std::endl << std::endl;

	for (auto i = 0uz; i < size_1; ++i)
	{
		for (auto j = 0uz; j < size_2; ++j)
		{
			for (auto k = 0uz; k < size_3; ++k)
			{
				std::cout << std::setw(2) << std::setfill(' ') << std::right;
				
				std::cout << array[i][j][k] << " ";
			}

			std::cout << std::endl;
		}

		std::cout << std::endl;
	}
}