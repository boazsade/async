#pragma once
#include "generator.hpp"

namespace async
{

template <typename T>
inline generator<T> multiply(generator<T>& g, T factor) 
{
  for (auto&& v: g) {
    co_yield v * factor;
  }
}

template <typename T> inline 
generator<T> sub(generator<T>& g, T adder) {
  for (auto&& v : g) {
    co_yield v - adder;
  }
}

template <typename T>
inline generator<T> divide(generator<T>& g, T factor) 
{
  for (auto&& v : g) {
    co_yield v / factor;
  }
}

template <typename T> inline 
generator<T> add(generator<T>& g, T adder) {
  for (auto&& v : g) {
    co_yield v + adder;
  }
}

}   // end of namespace async
