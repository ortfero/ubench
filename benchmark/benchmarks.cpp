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
