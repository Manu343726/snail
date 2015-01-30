
#include <manu343726/turbo_core/map.hpp>

namespace snail
{
	namespace categories
	{
		struct unknown {};

		struct unary_inmutable {};
		struct unary_mutable {};

		struct binary_inmutable {};
		struct binary_mutable {};

		struct unary_property {};
		struct binary_property {};
	}

	template<typename T>
	struct category
	{
		using result = categories::unknown;
	};

	#define algorithm_category(algorithm, c) \
											 \
			namespace snail { 				 \
	    		template<> 					 \
	    		struct category<SNAIL_ENTITY(algorithm)> : public tml::function<c> {}; \
			}
}