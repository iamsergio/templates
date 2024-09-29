#include <iostream>

constexpr int g = 10;

int main() {
    if constexpr(g == 10) {
        std::cout << "10\n";
    } else {
        std::cout << "not 10\n";
    }

    return 0;
}
