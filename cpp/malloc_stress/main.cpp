#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

const size_t MB = 1024 * 1024;

size_t parse_size(const std::string &arg)
{
    if (arg.empty()) {
        return 0;
    }

    size_t value = 0;
    char unit = arg.back();

    if (isdigit(unit)) {
        return std::stoull(arg);
    }

    value = std::stoull(arg.substr(0, arg.size() - 1));

    switch (unit) {
    case 'K':
    case 'k':
        value *= 1024;
        break;
    case 'M':
    case 'm':
        value *= 1024 * 1024;
        break;
    case 'G':
    case 'g':
        value *= 1024 * 1024 * 1024;
        break;
    default:
        std::cerr << "Unknown size unit: " << unit << std::endl;
        return 0;
    }

    return value;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <size>[K|M|G]" << std::endl;
        std::cerr << "Example: " << argv[0] << " 10G" << std::endl;
        return 1;
    }

    size_t total_size = parse_size(argv[1]);
    if (total_size == 0) {
        std::cerr << "Invalid size specification" << std::endl;
        return 1;
    }

    const size_t chunk_size = MB;
    size_t num_chunks = total_size / chunk_size;
    if (total_size % chunk_size != 0) {
        num_chunks++;
    }

    size_t allocated = 0;

    for (size_t i = 0; i < num_chunks; ++i) {
        size_t current_chunk_size =
            (i == num_chunks - 1 && total_size % chunk_size != 0)
            ? (total_size % chunk_size)
            : chunk_size;

        void *ptr = malloc(current_chunk_size);

        if (ptr == nullptr) {
            std::cerr << "Failed to allocate memory at chunk " << i
                      << " after allocating " << allocated << " bytes" << std::endl;
            break;
        }

        memset(ptr, 0xAB, current_chunk_size);

        allocated += current_chunk_size;

        if ((i + 1) % 100 == 0 || i == num_chunks - 1) {
            std::cout << "Progress: " << allocated << " bytes allocated ("
                      << (allocated * 100.0 / total_size) << "%)" << std::endl;
        }
    }

    std::cout << "Memory allocated. Sleeping forever..." << std::endl;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(3600));
    }

    return 0;
}
