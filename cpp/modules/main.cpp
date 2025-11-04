import my_module_example;

#include <iostream>

int main() {
    unsigned n{};
    std::cout << "Enter n (0..93 recommended): ";
    if (!(std::cin >> n)) {
        std::cerr << "Invalid input\n";
        return 1;
    }

    auto result = fibonacci(n);
    std::cout << "fib(" << n << ") = " << result << '\n';
    return 0;
}
