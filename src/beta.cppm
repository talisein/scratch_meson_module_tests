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

bool objects_are_cxx20_but_bmi_is_cxx23() {
  return widget::compiled_dialect() == 202002L && widget::interface_dialect() == 202302L;
}

const bool registered = [] {
  test::register_test("widget module vocabulary", &describe_uses_every_unit);
  test::register_test("C++20 objects, C++23 BMI", &objects_are_cxx20_but_bmi_is_cxx23);
  return true;
}();

}  // namespace
