// Registers alpha's tests with the `test` module. Importing this module is what
// runs its initializer, which is what puts the tests in the registry -- so
// test::run() does something different depending on which of these an executable
// imports.
export module alpha;

// Header units, at c++23; widget declares the same ones at c++20.
import <string>;
import <vector>;
import fmt;
import greeting;
import test;

namespace {

bool greeting_is_generated() {
  return greeting::greet("alpha") == "Hello, alpha!";
}

bool roster_is_formatted() {
  const auto lines = greeting::roster();
  return lines.size() == 3 && lines.front() == fmt::format("1. {}", greeting::greet("Ada"));
}

const bool registered = [] {
  test::register_test("greeting from a .cppm", &greeting_is_generated);
  test::register_test("roster through fmt", &roster_is_formatted);
  return true;
}();

}  // namespace
