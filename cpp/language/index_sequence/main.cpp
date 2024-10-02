#include <iostream>
#include <utility>

// fold-expression and std::index_sequence minimal repro

template<typename... Args, std::size_t... Indices>
void print_args_helper(const std::tuple<Args...> &t,
                       std::index_sequence<Indices...>)
{
    ((std::cout << std::get<Indices>(t) << " "), ...); // C++17 fold expression
}

template<typename... Args>
void print_args(const std::tuple<Args...> &t)
{
    print_args_helper(t, std::make_index_sequence<sizeof...(Args)> {});
}

int main()
{
    auto t = std::make_tuple(1, 2.5, "Hello");
    print_args(t); // Output: 1 2.5 Hello
    return 0;
}
