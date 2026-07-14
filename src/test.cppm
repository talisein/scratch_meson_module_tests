// A tiny test registry. This one source is compiled into *both* the test-alpha
// and test-beta executables, so the module `test` has two providers in this
// build. That is legal precisely because they are executables: nothing can link
// one, so the module is private by construction and each gets its own BMI.
export module test;

import <print>;
import <string_view>;
import <vector>;

export namespace test {

using test_fn = bool (*)();

void register_test(std::string_view name, test_fn fn);

// Runs whatever registered itself, so what this does depends entirely on which
// module the importing executable pulled in.
int run();

}  // namespace test

// Not an anonymous namespace: an entity with internal linkage is TU-local, and
// naming one from the module purview (std::vector<entry> instantiates templates
// that would be written into the BMI) is an exposure gcc 15 rejects outright.
// Unexported names in the purview have module linkage, which is what we want:
// invisible outside, but nothing about them is TU-local.
namespace test::detail {

struct entry {
  std::string_view name;
  test::test_fn fn;
};

std::vector<entry>& registry() {
  static std::vector<entry> tests;
  return tests;
}

}  // namespace test::detail

namespace test {

using detail::registry;

void register_test(std::string_view name, test_fn fn) {
  registry().push_back({name, fn});
}

int run() {
  int failed = 0;
  for (const auto& [name, fn] : registry()) {
    const bool ok = fn();
    std::println("{:<24} {}", name, ok ? "OK" : "FAILED");
    failed += ok ? 0 : 1;
  }
  std::println("{} test(s), {} failed", registry().size(), failed);
  return failed == 0 ? 0 : 1;
}

}  // namespace test
