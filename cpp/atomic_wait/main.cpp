#include <atomic>
#include <print>

int main() {
    
    std::atomic<bool> foo{false};
    
    foo.wait(false);
    
    std::println("waited!");
    
    return 0;
}
