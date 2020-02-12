#pragma once
#include "generator.hpp"

namespace async
{

// these function can be used inside lazy evaluated algorithm
const auto adjacent_difference = [&init_value](auto&& v) mutable {
    const auto diff{v - init_value};
    init_value = v;
    return diff;
};

// unlike number find_if, this would yield any value that matches
// as long as we have values to look for, it would return default value in case if faile to find
template<typename T>
generator<t> find(generator<T>& g, T what)
{
    for (auto&& v : g) {
        if (v == what) {
            co_yield v;
        }
    }
    co_return T{};
}

template<typename T, typename P>
generator<t> find_if(generator<T>& g, P&& predicate)
{
    for (auto&& v : g) {
        if (predicate(v)) {
            co_yield v;
        }
    }
    co_return T{};
}

// this would "return" a value if this is not what we don't want
// it would "filter out" any value from the generator that do match
// the value that was pass to it
template<typename T>
generator<T> remove(generator<T>& g, T val)
{
    for (auto&& v : g) {
        if (v != val) {
            co_yield v;
        }
    }
}

template<typename T, typename P>
generator<T> remove_if(generator<T>& g, P&& predicate)
{
    for (auto&& v : g) {
        if (not predicate(v)) {
            co_yield v;
        }
    }
}

template<typename T>
generator<T> fill(T val)
{
    while (true) {
        co_yield val;
    }
}

template<typename T>
generator<T> fill_n(T val, std::size_t n)
{
    for (auto i = 0u; i < n; ++i) {
        co_yield val;
    }
}

// remove duplications from a range, only if consecutive
template<typename T>
generator<T> unique(generator<T>& range)
{
    auto b{std::begin(range)};
    auto f{*b};
    ++b;
    for (auto&& v : range) {
        if (v != f) {
            co_yield v;
            f = v;
        }
    }

}

// remove duplications from a range, only if consecutive
template<typename T, typename P>
generator<T> unique(generator<T>& range, P&& predicate)
{
    auto b{std::begin(range)};
    auto f{*b};
    ++b;
    for (auto&& v : range) {
        if (not predicate(v, f)) {
            co_yield v;
            f = v;
        }
    }

}

template<typename T>
generator<T> partial_sum(generator<T>& range, T init_value = T{})
{
    for (auto&& v : range) {
        init_value += v;
        co_yield init_value;
    }
}

template<typename T, typename Op>
generator<T> partial_sum(generator<T>& range, Op&& operation, T init_value = T{})
{
    for (auto&& v : range) {
        init_value = operation(std::move(init_value), std::move(v));
        co_yield init_value;
    }
}



}   // end of namespace async

