#include <iostream>
#include <vector>
#include <memory>
#include <climits>
#include "foo.h"

#include <boost/smart_ptr/local_shared_ptr.hpp>

using TShared = boost::local_shared_ptr<benchmark::DummyStruct>;
// using TShared = std::shared_ptr<benchmark::DummyStruct>;

int main() {
    constexpr int VECTOR_SIZE = 5000;
    constexpr int ITERATIONS = 1000000;
    
    std::cout << "Creating vector of " << VECTOR_SIZE << " shared pointers to DummyStruct..." << std::endl;

    std::vector<TShared> vec;
    vec.reserve(VECTOR_SIZE);
    for (int i = 0; i < VECTOR_SIZE; ++i) {
        vec.push_back(TShared(new benchmark::DummyStruct()));
    }
    
    std::cout << "Vector created with " << vec.size() << " shared pointers" << std::endl;
    
    std::cout << "Starting " << ITERATIONS << " iterations..." << std::endl;
    long long totalSum = 0;
    
    for (int iteration = 0; iteration < ITERATIONS; ++iteration) {
#ifdef BENCHMARK_BY_VALUE
        for (const auto ptr : vec) {
#else
        for (const auto& ptr : vec) {
#endif
            totalSum += ptr->add(1);
        }
    }
    
    std::cout << "Total sum after " << ITERATIONS << " iterations: " << totalSum << std::endl;
    
    return static_cast<int>(totalSum % INT_MAX);
}