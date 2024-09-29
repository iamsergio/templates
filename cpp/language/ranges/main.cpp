#include <algorithm>
#include <iostream>
#include <ranges>
#include <vector>

int main()
{
    std::vector<int> vec = { 1, 3, 2 };
    std::ranges::sort(vec);

    std::cout << "sort result: vec[2]=" << vec[2] << "\n";

    auto output = vec | std::views::filter([](auto i) {
        std::cout << "filtering " << i << "\n";
        return i == 2; }) | std::views::transform([](auto i) { return i * 2; });

    std::cout << "starting to iterate\n";
    for (auto a : output)
        std::cout << a << "\n";

    return 0;
}
