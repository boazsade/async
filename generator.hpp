#pragma once
#include "coroutine.hpp"
#include <stdexcept>

namespace async
{
namespace stdx = std::experimental;

template <typename T> struct generator {
    struct promise_type {
            T current_value;
            stdx::suspend_always yield_value(T value) {
            this->current_value = value;
            return {};
        }

        stdx::suspend_always initial_suspend() { 
            return {}; 
        }

        stdx::suspend_always final_suspend() { 
            return {}; 
        }

        generator get_return_object() { 
            return generator{this}; 
        }

        void unhandled_exception() { 
            throw std::runtime_error{"unhandle exception"};
        }

        void return_void() {
        }
    };

  struct iterator {
    stdx::coroutine_handle<promise_type> _Coro;
    bool _Done;

    iterator(stdx::coroutine_handle<promise_type> coro, bool d) : 
        _Coro{coro}, _Done{d} 
    {
    }

    iterator &operator++() {
      _Coro.resume();
      _Done = _Coro.done();
      return *this;
    }

    bool operator==(iterator const &_Right) const {
      return _Done == _Right._Done;
    }

    bool operator!=(iterator const &_Right) const { 
        return !(*this == _Right); 
    }

    T const &operator*() const { 
        return _Coro.promise().current_value; 
    }

    T const *operator->() const { 
        return &(operator*()); 
    }
  };

  iterator begin() {
    p.resume();
    return {p, p.done()};
  }

  iterator end() { 
      return {p, true}; 
  }

  generator(generator const&) = delete;
  generator(generator &&rhs) : p(rhs.p) 
  { 
      rhs.p = nullptr; 
  }

  ~generator() {
    if (p)
      p.destroy();
  }

private:
  explicit generator(promise_type *p)
      : p(stdx::coroutine_handle<promise_type>::from_promise(*p)) 
  {
  }

  stdx::coroutine_handle<promise_type> p;
};

}   // end of namespace async
