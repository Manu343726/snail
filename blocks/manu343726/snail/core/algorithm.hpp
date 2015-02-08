#include <manu343726/turbo_core/cpp_meta_macros.hpp>
#include <manu343726/turbo_core/to_string.hpp>
#include <type_traits>

namespace snail
{
    //Dispatch algorithm factories indirectly to allow user-defined factories in namespaces of their own
    template<typename Algorithm, typename Category>
    auto dispatch(Algorithm algorithm, Category category)
    {
        using snail::algorithm_factory;
       
        return algorithm_factory(algorithm, category);
    }
}

#define algorithm_wrapper(algorithm) [](auto... args){ return algorithm(std::forward<decltype(args)>(args)...); }
#define make_algorithm(algorithm) snail::dispatch(algorithm_wrapper(algorithm), tml::eval<snail::category<SNAIL_ENTITY(algorithm)>>{})

template<typename C, typename F>
auto operator|(C&& container, F f)
{
    return std::move(f(std::forward<C>(container)));
}
