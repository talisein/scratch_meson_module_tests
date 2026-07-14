// The same idea against the C++20 widget library, so beta's test::run() reports
// on the BMI flag classes instead.
export module beta;

import <print>;
import <string>;
import widget;
import test;

namespace {

bool describe_uses_every_unit() {
  // partition (render) + internal partition (name_for) + private module (stamp)
  const auto text = widget::describe(4);
  return text.starts_with("square (4 sides)") && text.contains("secret");
}

// Not a literal for the newer dialect: cl's /std:c++latest reports its own value.
bool objects_are_cxx20_but_bmi_is_newer() {
  return widget::compiled_dialect() == 202002L &&
         widget::interface_dialect() > widget::compiled_dialect();
}

const bool registered = [] {
  test::register_test("widget module vocabulary", &describe_uses_every_unit);
  test::register_test("c++20 objects, newer BMI", &objects_are_cxx20_but_bmi_is_newer);
  return true;
}();

}  // namespace
