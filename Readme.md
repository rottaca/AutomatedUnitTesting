# Automated Unit Test Framework (C++20)

This repository contains a rudimentary test framework which is capable to auto-generate
unit tests, based on constrained function arguments and return types.

This is a free-time project in order to practice my C\++11 to C\++20 skills in template meta programming.
It is for sure not a complete and and ready-to-use framework but it helped me a lot to refresh my knowledge 
and also to practice newly learned techniques.

The main goal was to generate the test-cases at compile-time. Also, the constraint wrapper classes should
not have a noticeable runtime overhead.

## Features
This repository allows testing functions, with simple plain-old datatype arguments.
These arguments and also the return type can be decorated with constraints which specify the valid value range
for these values. 

```c++
// int fib(int n) {
aut::greater<0, int> fib(aut::greater<0, int> n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;

    int f1 = 1;
    int f2 = 1;
    for (int i = 2; i < n; ++i)
    {
        int fneu = f2 + f1;
        f1 = f2;
        f2 = fneu;
    }
    return f2;
}
```

By using template meta programming, these constrains can be extracted, interpreted and
test-cases can be derived. Then, the actual return value is compared with the specified constraints 
and a pass/fail verdict is printed.

```c++
#include <aut/testgenerator.hpp>

int main() {
    aut::test_func<fib>();
}
```

Tests can be generated for:

- Global functions
- Lambda functions and other functors
- Static class member functions
- **TODO:** Member functions