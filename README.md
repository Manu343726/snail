Snail
=====

Continuation-ready algorithms from STL algorithms

Motivation
----------

The C++ Standard Library comes with a very well suitted set of generic algotihms. The iterator-range design of such algorithms decouples them completely from the underlying data they are operating on,
which makes possible to apply such algorithms on any entity, if a compatible iterator interface is provided. 

My favorite example are stream iterators:

	auto numbers = {1, 2, 3, 4, 5};

    std::copy(std::begin(numbers), std::end(numbers), 
              std::ostream_iterator<std::string>(std::cout, " "));

The above snippet prints the set of numbers on the standard output. It "copies" the numeric sequence to a sequence of output operations represented by a `std::ostream_iterator` instance.

But everything is not that simple and awesome. The iterator-range interface comes at a high cost: **Is not easy to chain multiple STL algorithms**.  Whenever you want to apply multiple algorithms setp by sep the process becomes a bit clumsy:

    std::vector<int> v = {1, 2, 3, 5, 7, 11};
    decltype(v) w;

    std::copy_if(std::begin(v), std::end(v), 
                 std::back_inserter(w), 
                 [](int x){ return is_prime(x); });

    std::swap(v,w);
    w.clear();

    std::sort(std::begin(v), std::end(v), std::less<int>{});

Compare that to the more natural fluent interface operating on the same vector along the pipeline of operations:

    v.filter([](int x){ return is_prime(x); })
     .sort(std::less<int>{})

Snail is my try to get a continuation-ready set of algorithms to operate on C++ containers, but instead of reinventing all the algorithms, addapting them though a continuation monad (Or something resembling a continuation monad).

Design choices
--------------

Monads. Everybody in the C++ community is talking about functional programming these days. Doing functional constructions in C++ is not as pretty as in Haskell, but current language standards (C++11 and C++14) help a lot on this matter. Lambdas, `auto`, function return type deduction, etc.

The inspiration for this library comes mainly from the awesome Joaquín M López Muñoz's [*"A workshop on monads with C++14"*](https://github.com/joaquintides/cpp14monadworkshop) and its exercises about the continuation monad.

Snail is based in several facts:

 - **Use existing algorithms**: Make a mechanism to reuse the existing algorithms from the Standard Library, instead of hand-writting such algorithms again with a different interface.

 - More?