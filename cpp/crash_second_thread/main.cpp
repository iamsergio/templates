#include <iostream>
#include <thread>
#include <chrono>
#include <pthread.h>

void crash_thread(bool should_crash) {
    pthread_setname_np(pthread_self(), "CrashWorker");
    std::cout << "Thread 'CrashWorker' started" << std::endl;
    
    if (should_crash) {
        int* ptr = new int(42);
        std::cout << "CrashWorker: Allocated memory: " << *ptr << std::endl;
        
        delete ptr;
        std::cout << "CrashWorker: First delete completed" << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        std::cout << "CrashWorker: About to perform double-free..." << std::endl;
        delete ptr;
        std::cout << "CrashWorker: This line should not be reached" << std::endl;
    } else {
        std::cout << "CrashWorker: Running normally, no crash" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "CrashWorker: Completed successfully" << std::endl;
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
        std::cout << "MainThread: About to crash main thread..." << std::endl;
        int* ptr = new int(42);
        std::cout << "MainThread: Allocated memory: " << *ptr << std::endl;
        delete ptr;
        std::cout << "MainThread: First delete completed" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "MainThread: About to perform double-free..." << std::endl;
        delete ptr;
        std::cout << "MainThread: This line should not be reached" << std::endl;
    } else {
        std::thread worker_thread(crash_thread, true);
        std::cout << "MainThread: Waiting for worker thread to complete..." << std::endl;
        worker_thread.join();
    }
    
    std::cout << "MainThread: Program completed normally" << std::endl;
    return 0;
}