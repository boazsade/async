#pragma once
#include "generator.hpp"

namespace async
{

template<typename T>
generator<T> take_some(generator<T>& gen, std::size_t limit)
{
    auto count{0u};
    for (auto&& v : gen) {
        if (count == limit) {
            break;
        }
        ++count;
        co_yield v;
    }
}

template <typename T, typename L>
generator<T> take_until(generator<T>& g, L limit) 
{
    for (auto&& v: g) {
        if (v < limit) {
	    co_yield v;
        } else { 
	    break;
        }
    }
}

template <typename T, typename P>
generator<T> take_until_if(generator<T>& g, P&& condition) 
{
    for (auto&& v: g) {
        if (condition(v)) {
	    co_yield v;
        } else { 
	    break;
        }
    }
}

}   // end of namespace async
