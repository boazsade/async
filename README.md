This is an example for using the async API
Please note that what this mean is that you are creating a 
list of operations, then the exectution of these is done
only when the resutls are required

To build this you need to add -fcoroutines-ts
Please note that as of c++17 this is not supported as part of the language, but 
it is supported on clag 5 and above (using the above compilation flag
```c++
#include <iostream>
//#include "async/algorithm.hpp"
#include "async/sequence.hpp"
#include "async/math_ops.hpp"
#include "async/take_until.hpp"
#include "async/generator.hpp"
#include <numeric>
#include <vector>
#include <iterator>
#include <algorithm>

auto produce_array = [] (int start, int step, std::size_t size) {
	return [ start, step, size ]() -> async::generator<std::vector<int>> {
	    auto seq{async::sequence<int>(start, step)};
	    std::vector<int> values(size, int{});
	    std::size_t counter{0};
	    for (auto&& n : seq) {
	        if (counter == size) {
		    co_yield values;
		    counter = 0;
		} else {
		    values[counter++] = n;
	      }
	   }
	};
};

int main() {
	using namespace async;
  auto s{sequence<double>(21.4, 3.3)}; 
  auto t{take_until(s, 220)}; 
  auto m{multiply(t, 2.0)};
  auto a = add(m, 110.0);
  std::cout<<"result of sum = "<< std::accumulate(std::begin(a), std::end(a), 0)<<std::endl;
  // create an endless sequence and print it out
  auto se{sequence<double>(1.2, 5.0)};
  auto j = 0;
  for (auto&& c : se) {
	std::cout<<c<<", ";
	if (j++ > 1000) {
		std::cout<<std::endl;
		break;
	}
  }
  // create sequece of buffers and print them
  // note that the sequnce that is produce is diffrent values in each iteration
  auto producer{produce_array(1, 2, 5)};
  auto i = 0;
  for (auto&& v : producer()) {
      std::cout<<"got vector of size "<<v.size()<<" with values = \n";
      std::copy(std::begin(v), std::end(v), std::ostream_iterator<int>(std::cout, ", "));
      std::cout<<"\n";
      if (i++ > 10) {
	      break;
      }
  }

  auto s1 = sequence<double>(3.4, 5.3);
  auto t1 = take_until(s1, 22);
  auto ad{transform<double>(std::move(t1), [start=3.4] (auto&& v) mutable { 
		  auto diff{v - start}; 
		  start = v; 
		  return diff; 
	}
     )
  };
  for (auto&& s : ad) {
	  std::cout<<"'"<<s<<"' ";
  }
  //std::copy(std::begin(tr), std::end(tr), std::ostream_iterator<std::string>(std::cout, "\n"));
  std::cout<<std::endl;
}

```
