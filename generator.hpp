#pragma once
#if defined(__clang__)  // make sure to add -stdlib=libc++
#   include <experimental/coroutine>
#   define std_ns std::experimental
#else
#   include <coroutine>
#   define std_ns std
#endif  // __clang__
#include <stdexcept>

namespace async
{
//namespace stdx = std::experimental;

template <typename T> 
struct generator {
    struct promise_type {
            T current_value;
            std_ns::suspend_always yield_value(T value) {
            this->current_value = value;
            return {};
        }

        std_ns::suspend_always initial_suspend() { 
            return {}; 
        }

        void return_value(T val) {
            this->current_value = val;
        }

        std_ns::suspend_always final_suspend() { 
            return {}; 
        }

        generator get_return_object() { 
            return generator{this}; 
        }

        void unhandled_exception() { 
            throw std::runtime_error{"unhandle exception"};
        }

        void RETURN_VALUE() {
        }
    };

  struct iterator {
    std_ns::coroutine_handle<promise_type> _Coro{};
    bool _Done{true};

    iterator(std_ns::coroutine_handle<promise_type> coro, bool d) : 
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
    if (p) {
      p.destroy();
    }
  }

private:
  explicit generator(promise_type *p)
      : p(std_ns::coroutine_handle<promise_type>::from_promise(*p)) 
  {
  }

  std_ns::coroutine_handle<promise_type> p;
};

}   // end of namespace async
