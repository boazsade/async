#pragma once
#include "generator.hpp"
#include "coroutine.hpp"

namespace async
{

template<typename T, typename Op>
generator<T> binary_apply(generator<T>& g, Op&& operation)
{
    for (auto&& v : g) {
	co_yield operation(v);
    }
}

}   // end of namespace async
