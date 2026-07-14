// Implementation unit: `module widget;`, no export, plain .cpp and no kwarg.
// The scan sees it belongs to widget and orders it after the primary's BMI.
module;

#include "dialect.h"

module widget;

import :detail;
import widget_secret;
import fmt;
// Header units instead of `import std;` -- see meson.build. c++20 here.
import <string>;
import <vector>;

namespace widget {

long compiled_dialect() { return WIDGET_DIALECT; }

std::string describe(int sides) {
  const auto s = shape{detail::name_for(sides), sides};
  const std::vector<std::string> parts{render(s), widget_secret::stamp()};
  return fmt::format("{} {}", parts.front(), parts.back());
}

}  // namespace widget
