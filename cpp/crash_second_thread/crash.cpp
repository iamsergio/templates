#include <thread>
#include <chrono>

extern void crash_indirection(int *data);

void make_crash() {
    int* ptr = new int(42);
    delete ptr;
    crash_indirection(ptr);
}

void wait_forever() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}