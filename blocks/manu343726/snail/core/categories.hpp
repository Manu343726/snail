
#include <manu343726/turbo_core/map.hpp>

namespace snail
{
	namespace categories
	{
		struct unknown {};
	}

	template<typename T>
	struct category
	{
		using result = categories::unknown;
	};
}

#define algorithm_category(algorithm, c) \
                                         \
        namespace snail { 				 \
            template<> 					 \
            struct category<SNAIL_ENTITY(algorithm)> : public tml::function<c> {}; \
        }

