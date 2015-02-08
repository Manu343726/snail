namespace snail
{
    namespace categories
    {
        struct unary_inmutable {};
		struct unary_mutable {};

		struct binary_mutable {};

		struct unary_property {};
		struct binary_property {};
    }
    
    template<typename Algorithm>
    auto algorithm_factory(Algorithm algorithm, snail::categories::unary_inmutable)
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
    
    template<typename Algorithm>
    auto algorithm_factory(Algorithm algorithm, snail::categories::unary_mutable)
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
    
    template<typename Algorithm>
    auto algorithm_factory(Algorithm algorithm, snail::categories::binary_mutable)
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
    
    template<typename Algorithm>
    auto algorithm_factory(Algorithm algorithm, snail::categories::unary_property)
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
    
    template<typename Algorithm>
    auto algorithm_factory(Algorithm algorithm, snail::categories::binary_property)
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
}

