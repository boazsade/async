#pragma once
#include "generator.hpp"

namespace async
{

template <typename T>
generator<T> sequence(T start, T step) 
{
    auto i{start};
    while (true) {
        co_yield i;
	i += step;
    }
}

template <typename T>
generator<T> sequence(T start = T{})
{
    while (true) {
        co_yield start;
	++start;
    }
}

template<typename T, typename G>
generator<T> sequence(G&& gen_op) 
{
    while (true) {
        co_yield gen_op();
    }
}

template<typename T, typename S, typename F>
generator<T> transform(S seq, F op)
{
    for (auto&& v : seq) {
        co_yield op(v);
    }
}

template<typename T, typename S>
generator<T> transform(S seq)
{
    for (auto&& v : seq) {
        co_yield v;
    }
}

}   // end of namespace async
