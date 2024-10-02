#include <expected>
#include <iostream>

enum class MyError {
    Error1,
    Error2
};

std::expected<unsigned int, MyError> calculateAge()
{
    // return {}; returns value=0
    // return 41; // returns value=41
    return std::unexpected(MyError::Error1);
}

// Error handling with std::variant instead:
struct Error1
{
    int v = 1;
};

struct Error2
{
};
using MyErrorVariant = std::variant<Error1, Error2>;

std::expected<unsigned int, MyErrorVariant> calculateAge2()
{
    // return {}; returns value=0
    return 41; // returns value=41
    // return std::unexpected(MyError::Error1_);
}

template<class... Ts>
struct overload : Ts...
{
    using Ts::operator()...;
};


int main()
{
    auto result = calculateAge();
    std::cout << "result = " << result.value_or(42) << "\n";

    if (result) {
        std::cout << "success\n";
    } else {
        MyError err = result.error();
        std::cout << "error\n";
    }

    auto result2 = calculateAge2();
    if (result2) {
        std::cout << "success\n";
    } else {
        std::visit(overload { [](Error1) { std::cout << "Error1\n"; },
                              [](Error2) {
                                  std::cout << "Error2\n";
                              } },
                   result2.error());
    }

    return 0;
}
