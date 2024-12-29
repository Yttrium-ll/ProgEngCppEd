#include <cmath>
#include <limits>
#include <string>

#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////////

using float_100_t = boost::multiprecision::cpp_bin_float_100;

//////////////////////////////////////////////////////////////////////////////////////////////////

template < typename F > auto derivative(F && f, float_100_t x, float_100_t dx = 1e-9)
{
    auto d1 = f(x + 1 * dx) - f(x - 1 * dx);
    auto d2 = f(x + 2 * dx) - f(x - 2 * dx);
    auto d3 = f(x + 3 * dx) - f(x - 3 * dx);

    return (15 * d1 / 2 - 6 * d2 / 4 + 1 * d3 / 6) / (10 * dx);
}

//////////////////////////////////////////////////////////////////////////////////////////////////

template < typename T > auto equal(T x, T y, T epsilon = std::numeric_limits < T > ::epsilon())
{
	return boost::multiprecision::abs(x - y) < epsilon;
}

//////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    auto x = float_100_t("0." + std::string(std::numeric_limits < float_100_t > ::digits10, '3'));

    auto y = float_100_t(1.0) / 3.0;

    assert(equal(x, y));

    auto z = boost::math::constants::pi < float_100_t > () / 2;

//  ----------------------------------------------------------------------------------------------

    auto d11 = derivative
    (
        [](const auto & x){ return boost::multiprecision::sin(x); }, z
    );

    auto d12 = float_100_t(0.0);

    assert(equal(d11, d12));

//  ----------------------------------------------------------------------------------------------

    auto d21 = derivative
    (
        [](const auto & x){ return boost::math::gamma_p (2.0, x); }, z
    );

    auto d22 = boost::math::gamma_p_derivative(2.0, z);

    assert(equal(d21, d22, float_100_t("0." + std::string(55, '0') + '1')));
}