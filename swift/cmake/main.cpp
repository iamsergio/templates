
extern "C" {
    int fibo(int);
    int fibo_static(int);
}

#include <iostream>

int main()
{
    std::cout << fibo(10) << "; " << fibo_static(10) << "\n";
    return 0;
}
