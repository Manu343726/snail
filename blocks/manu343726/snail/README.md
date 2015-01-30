Snail [![Build Status](https://webapi.biicode.com/v1/badges/manu343726/manu343726/snail/master)](https://www.biicode.com/manu343726/snail) [![Build Status](https://travis-ci.org/Manu343726/snail.svg)](https://travis-ci.org/Manu343726/snail)
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

 - **Use existing algorithms**: Write a mechanism to reuse the existing algorithms from the Standard Library, instead of hand-writting such algorithms again with a different interface.

 - **Simple syntax**: Allow consecutive algorithm calls on the same container, with a simple pipe-like syntax built on top of a continuation monad. This is not something new, see Boost.Range or Eric Niebler's range-v3 container algorithms. My point here is to have a working set of pipeable algorithms with a couple of lines only, using existing STL algorithms.

I should recall (again) that the point of this library is not to make an awesome container-algortihms pipeline. What I'm trying to do is to have a working container-algorithms pipeline without many effort, without having to rewrite those algorithms. Just take the STL.

Requeriments
------------

This library relies heavily on function return type deduction and generic lambdas. Any C++14 compliant compiler is fine.  

Also, the library is designed to be used with [biicode](http://www.biicode.com/), it has some external dependencies such as [string_id](http://www.biicode.com/foonathan/string_id). Is far easier to rely on a dependency manager and just `#include` what I need than having pain with git submodules and related.

Usage
-----

STL algorithms operate on ranges, ranges represented by iterator pairs `[begin, end)`. But our algorithms should operate on containers instead.  
The first thing we need is a [functor](http://en.wikipedia.org/wiki/Functor) to translate STL algorithms from the category of "*functions operating on iterator ranges*" to "*functions operating on containers*". That's the macro `make_algorithm()`:

``` cpp
auto sort = make_algorithm(std::sort);

auto w = sort(v);
```

Of course is not that easy. Different STL algorithms take different number of parameters and operate on ranges in a different way. Some algorithms are mutable on the input range, some others take two ranges (input/output), etc. We should identify the category of the target algorithm to take the appropiate functor.   
The macro `algorithm_category(algorithm, category)` specifies the category of an algorithm, so when the user requests the container-algorithm equivalent calling `make_algorithm()` the correct functor is applied.

``` cpp
algorithm_category(std::sort, snail::categories::unary_mutable)

auto sort = make_algorithm(std::sort);

auto w = sort(v);
```

`std::sort()` takes only one iterator range and mutates it, hence the `snail::categories::unary_mutable`. Snail defines a simple set of categories inside the `snail::categories` namespace.

Here's a complete example:

``` cpp
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

    v = std::move(v) | sort([](int x, int y){ return x < y; }) 
                     | map([](int x){ return x*3; }) 
                     | for_each([&](int x){ std::cout << x << " "; }) 
                     | filter([](int x){ return x % 2 != 0; })
                     | sort([](int x, int y){ return x > y; });

    std::cout << std::endl;

    v | for_each([&](int x){ std::cout << x << " "; });

    std::cout << std::endl;
}
```

Note that container algorithms are designed to be used within the continuation. For example, the `binary_mutable` category translates algorithms taking an input range writting some transformation on an output range, generating a container algorithm that takes a container and outputs another container. Since the continuation starts with one specific container, the output container type of `binary_mutable` algorithms is the same as its input by default, to make the continuation *continue* in a natural way.

Why snail?
----------

I have always had problems when naming projects. "*Containuation*"? Let's get simpler and choose a random animal name :)

![](https://raw.githubusercontent.com/Manu343726/snail/master/snail.jpg)

Judge the library by it's features, not by its name.

