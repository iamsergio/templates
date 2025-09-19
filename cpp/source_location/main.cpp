#include <iostream>
#include <source_location>
#include <vector>

static std::vector<std::source_location> s_locations;

struct Location {
  Location(std::source_location loc) { s_locations.push_back(loc); }

  ~Location() { s_locations.pop_back(); }

  Location(const Location &) = delete;
  Location &operator=(const Location &) = delete;
  Location(Location &&) = delete;
  Location &operator=(Location &&) = delete;

  static void print_stacktrace(Location l = std::source_location::current()) {
    for (const auto &loc : s_locations) {
      std::cout << loc.file_name() << ":" << loc.line() << " in "
                << loc.function_name() << std::endl;
    }
  }
};

void foo(Location l = std::source_location::current()) {
  Location::print_stacktrace();
}
void foo1(Location l = std::source_location::current()) { foo(); }
void foo2(Location l = std::source_location::current()) { foo1(); }

int main() {
  foo2();
  return 0;
}
