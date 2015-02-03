#include <manu343726/turbo_core/cpp_meta_macros.hpp>
#include <manu343726/turbo_core/to_string.hpp>
#include <type_traits>

namespace snail
{
	template<typename T>
	struct algorithm_factory
	{
		static_assert(sizeof(T) != sizeof(T), "Non-implemented algorithm category");
	};

	template<>
	struct algorithm_factory<snail::categories::unary_inmutable>
	{
		template<typename Algorithm>
		static auto make(Algorithm algorithm)
		{
			return [=](auto f)
			{
				return [=](const auto& container) -> decltype(container)
				{
					using std::begin;
					using std::end;
					algorithm( begin(container), end(container), f);

					return container;
				};
			};
		}
	};

	template<>
	struct algorithm_factory<snail::categories::unary_mutable>
	{
		template<typename Algorithm>
		static auto make(Algorithm algorithm)
		{
			return [=](auto f)
			{
				return [=](auto container)
				{
					using std::begin;
					using std::end;
					algorithm( begin(container), end(container), f);

					return std::move(container);
				};
			};
		}
	};

	template<>
	struct algorithm_factory<snail::categories::binary_mutable>
	{
		template<typename Algorithm>
		static auto make(Algorithm algorithm)
		{
			return [=](auto f)
			{
				return [=](const auto& container)
				{
					using std::begin;
					using std::end;
					using std::back_inserter;
					tml::eval<std::remove_cv<std::remove_reference<decltype(container)>>> out;

					algorithm( begin(container), end(container),
					           back_inserter(out), f);

					return std::move(out);
				};
			};
		}
	};

	template<>
	struct algorithm_factory<snail::categories::unary_property>
	{
		template<typename Algorithm>
		static auto make(Algorithm algorithm)
		{
			return [=](auto f)
			{
				return [=](const auto& container)
				{
					using std::begin;
					using std::end;
					return algorithm( begin(container), end(container), f);
				};
			};
		}
	};

	template<>
	struct algorithm_factory<snail::categories::binary_property>
	{
		template<typename Algorithm>
		static auto make(Algorithm algorithm)
		{
			return [=](auto f, const auto& sequence)
			{
				return [=](const auto& container)
				{
					using std::begin;
					using std::end;
					return algorithm( begin(container), end(container),
					                  begin(sequence), f);
				};
			};
		}
	};

	#define algorithm_wrapper(algorithm) [](auto... args){ return algorithm(std::forward<decltype(args)>(args)...); }
	#define make_algorithm(algorithm) snail::algorithm_factory<tml::eval<snail::category<SNAIL_ENTITY(algorithm)>>>::make(algorithm_wrapper(algorithm))
}

template<typename C, typename F>
auto operator|(C&& container, F f)
{
    return std::move(f(std::forward<C>(container)));
}
