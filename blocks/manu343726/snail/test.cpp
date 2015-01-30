#include <iostream>
#include <manu343726/snail/core.hpp>
#include <algorithm>

algorithm_category(std::sort     , snail::categories::unary_mutable)
algorithm_category(std::for_each , snail::categories::unary_inmutable)
algorithm_category(std::transform, snail::categories::binary_mutable)
algorithm_category(std::copy_if  , snail::categories::binary_mutable)

auto sort     = make_algorithm(std::sort);
auto for_each = make_algorithm(std::for_each);
auto map      = make_algorithm(std::transform);
auto filter   = make_algorithm(std::copy_if);

int main()
{
	std::vector<int> v = {1, 2, 3, 4, 5};

	v = std::move(v) | sort([](int x, int y){ return x < 2; }) 
	                 | map([](int x){ return x*3; }) 
	                 | for_each([&](int x){ std::cout << x << " "; }) 
	                 | filter([](int x){ return x % 2 != 0; })
	                 | sort([](int x, int y){ return x > y; });

	std::cout << std::endl;

	v | for_each([&](int x){ std::cout << x << " "; });

	std::cout << std::endl;
}
