#include <iostream>
#include <thread>

#include <ubench/ubench.hpp>



int main() {
    using namespace std;

    int x                   = 0;
    auto const fast_benched = ubench::run([&] { ++x; });
    cout << "fast operation - " << fast_benched << '\n';

    if(fast_benched.time > chrono::nanoseconds {10})
        cout << "probably not so fast" << endl;

    auto const slow_benched = ubench::run<10>(
        [&] { this_thread::sleep_for(chrono::milliseconds {10}); });

    cout << "slow operation - " << slow_benched << '\n';

    return 0;
}
