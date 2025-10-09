#include <atomic>
#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>

// usage example: ./a.out 1  (to crash main thread)
//                ./a.out 2  (to crash worker thread)

std::atomic<int> flag(0);

extern void make_crash();
extern void wait_forever();

void crash_indirection(int *data) {
    delete data;
}

void crash_thread(bool should_crash) {
    pthread_setname_np(pthread_self(), "Thread2");
    std::cout << "Thread 'Thread2' started shouldcrash=" << should_crash << std::endl;
    
    if (should_crash) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        make_crash();
    } else {
        flag.store(1);
        flag.notify_all();

        wait_forever();
    }
}

int main(int argc, char* argv[]) {
    pthread_setname_np(pthread_self(), "MainThread");
    std::cout << "Thread 'MainThread' started" << std::endl;
    
    int crash_thread_num = 2;
    if (argc > 1) {
        crash_thread_num = std::stoi(argv[1]);
    }
    
    std::cout << "MainThread: Crash thread number: " << crash_thread_num << std::endl;
    
    if (crash_thread_num == 1) {
        std::thread worker_thread(crash_thread, false);
        flag.wait(0);
        make_crash();
    } else {
        std::thread worker_thread(crash_thread, true);
        std::cout << "MainThread: Waiting for worker thread to complete..." << std::endl;
        wait_forever();
    }
    
    std::cout << "MainThread: Program completed normally" << std::endl;
    return 0;
}