#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <boost/pool/object_pool.hpp>

#define N 1000000

struct Obj {
    char buffer[250];
};

void benchmark(bool free_objs = true)
{
    std::vector<Obj*> objects;
    objects.reserve(N * 10);
    
    for (int i = 0; i < 10; ++i)
        for (int i = 0; i < N; ++i) {
            auto obj = new Obj();
            objects.push_back(obj);
            
            auto cruft = new Obj();
            delete cruft;
        }

    if (free_objs) {
        for (Obj* obj : objects) {
            delete obj;
        }
    }
}

void benchmark_boost()
{
    std::vector<Obj*> objects;
    objects.reserve(N * 10);
    
    // Create a memory pool for Obj objects
    boost::object_pool<Obj> obj_pool {10000, 0};
    
    for (int i = 0; i < 10; ++i)
        for (int i = 0; i < N; ++i) {
            // Allocate from pool and construct object
            objects.push_back(obj_pool.construct());
            
            auto cruft = new Obj();
            delete cruft;
        }

    // Pool destructor automatically frees all memory when it goes out of scope
}

void print_help() {
    std::cout << "Usage: program [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --boost      Run the benchmark function with Boost memory pool\n";
    std::cout << "  --no-free    Don't delete objects after allocation\n";
    std::cout << "  --help       Show this help message\n";
    std::cout << "\n";
    std::cout << "Default behavior (no options): Run standard benchmark with glibc allocator\n";
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    bool boost_flag = false;
    bool free_objs_flag = true;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--boost") == 0) {
            boost_flag = true;
        } else if (strcmp(argv[i], "--no-free") == 0) {
            free_objs_flag = false;
        } else if (strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        }
    }

    if (boost_flag) {
        benchmark_boost();
    } else {
        // Default behavior: run glibc benchmark
        benchmark(free_objs_flag);
    }

    return 0;
}
