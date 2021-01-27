# ubench

[![License](https://img.shields.io/github/license/ortfero/ubench](LICENSE)
[![Build Status on GitHub Actions](https://github.com/ortfero/ubench/workflows/MSBuild/badge.svg)](https://github.com/ortfero/ubench/actions)

C++ 11 one-header library for dead simple micro benchmarking


## Snippets


### Benchmark

```cpp
#include <iostream>
#include <ubench/ubench.hpp>


int main() {
	int x = 0;
	ubench::result const inc_bench = ubench::run([&] {
		++x;
	});
	std::cout << "inc: " << inc_bench << std::endl;
	return 0;
}
```


### Get benchmark result in nanoseconds

```cpp
#include <cstdio>
#include <ubench/ubench.hpp>


int main() {
	int x = 0;
	auto [code, time] = ubench::run([&] {
		++x;
	});
	if(code != ubench::result_code::ok)
		std::printf("%s\n", ubench::describe(code));
	else
		std::printf("inc: %.1f\n", time.count());
	return 0;
}
```

Possible output:
```
inc - 3.4 ns
```


## Installation

Just place `ubench/ubench.hpp` somewhere at include path.


## Supported platforms and compilers

ubench requires C++ 11 compiler.


## License

ubench licensed under [MIT license](https://opensource.org/licenses/MIT).
