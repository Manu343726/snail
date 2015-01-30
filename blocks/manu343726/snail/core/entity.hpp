#include <manu343726/turbo_core/integral.hpp>

namespace snail
{	
	template<snail::hash_t hash>
	using entity = tml::integral_constant<snail::hash_t,hash>;

	#define SNAIL_ENTITY(x) snail::entity<SNAIL_HASH(x)>
}