#include "lib.h"

#include <any>
#include <iostream>

struct S
{
    S()
    {
        std::cout << "CTOR\n";
    }

    S(const S &)
    {
        std::cout << "copy-CTOR\n";
    }

    S(S &&)
    {
        std::cout << "move-CTOR\n";
    }

    ~S()
    {
        std::cout << "DTOR\n";
    }
};

int main()
{
    std::cout << "sizeof(any): " << sizeof(std::any) << "\n";
    S s;
    std::cout << "Creating any\n";
    {
        std::any v = s;
    }
    std::cout << "any destroyed\n";

    std::cout << "Passing any to a library\n";
    print_any(S());
    std::cout << "print_any called\n";
}
