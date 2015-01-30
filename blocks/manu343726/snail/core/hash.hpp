
#include <manu343726/turbo_core/algorithm.hpp>
#include <foonathan/string_id/hash.hpp>

namespace snail
{
	using hash_t = foonathan::string_id::hash_type;
}

#define SNAIL_HASH(x) foonathan::string_id::detail::sid_hash(#x)