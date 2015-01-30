#include <utility>

namespace snail
{
	template<typename C>
	struct container_simple : public C
	{
	    using C::C;
	    
	    template<typename A>
	    auto operator()(A algorithm)
	    {
	        return (*this) | algorithm;   
	    }

	    template<typename A, typename... Args>
	    auto operator()(A algorithm, Args&&... args)
	    {
	        return (*this) | algorithm(std::forward<Args>(args)...);   
	    }
	};
}