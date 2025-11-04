export module my_module_example;

export unsigned long long fibonacci(unsigned n) {
    if (n <= 1) return n;
    unsigned long long a = 0, b = 1;
    for (unsigned i = 2; i <= n; ++i) {
        unsigned long long c = a + b;
        a = b;
        b = c;
    }
    return b;
}