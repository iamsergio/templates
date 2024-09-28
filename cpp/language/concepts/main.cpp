#include <concepts>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>

// Simple
template <typename T>
concept integral = std::is_integral_v<T>;

// With requires expression
template <typename T>
concept ILabel = requires(T v) {
  // Requires that T has a member buildHtml() which returns something
  // convertible to string
  { v.buildHtml() } -> std::convertible_to<std::string>;
};

// With more requirements. Return types are not specified.
template <typename T>
concept Clock = requires(T c) {
  c.start();
  c.stop();
  c.getTime();
};

template <typename T>
  requires std::integral<T> || std::floating_point<T>
constexpr double Average(const std::vector<T> &vec) {
  const double sum = std::accumulate(vec.begin(), vec.end(), 0.0);
  return sum / vec.size();
}

// Shortcut:
// template <std::integral T> auto sum(const std::vector<T> &vec) {
//   // return ...;
// }

// Shortcut:
// auto func(concept auto param) { }
// example:
auto func(integral auto param) { return param; }

int main() {
  std::vector ints{1, 2, 3, 4, 5};
  std::vector<std::string> strs;
  double av = Average(ints);
  //   auto wrong = Average(strs); uncomment to see nice diagnostics
}