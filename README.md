# ubench
C++17 one-header library for dead simple micro benchmarking

# snippet

```cpp
#include <iostream>
#include <thread>

#include <ubench/ubench.hpp>



int main() {
  using namespace std;
  
  int x = 0;
  auto const fast_benched = ubench::run([&]{
    ++x;
  });
  cout << "fast operation - " << fast_benched
       << " [took " << fast_benched.took.count() << " us]" << endl;
  
  if(fast_benched.time > chrono::nanoseconds{10})
    cout << "probably not so fast" << endl;
  
  auto const slow_benched = ubench::run([&]{
    this_thread::sleep_for(chrono::milliseconds{10});
  });
  cout << "slow operation - " << slow_benched
       << " [took " << slow_benched.took.count() << " us]" << endl;

  return 0;
}
```

Output on my machine:
```
fast operation - 3.4 ns [took 84 us]
slow operation - 10771087.0 ns [took 226550 us]
```
