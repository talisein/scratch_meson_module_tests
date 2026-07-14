#include "compat.hpp"
#include "widget/dialect.h"

import std;
import greeting;
import widget;
import fmt_using_library;
import "cpr/cpr.h";

int main()
{
  std::println("{}", greeting::greet("modules"));
  for (const auto& line : greeting::roster()) {
    std::println("{}", fmt_using_library::make_amazing(line));
  }

  // widget is a C++20 library. Its interface was re-BMI'd for this C++23 flag
  // class, so interface_dialect() reports our dialect; its objects were built
  // once, at C++20, so compiled_dialect() reports theirs.
  std::println("main compiled at:          {}", WIDGET_DIALECT);
  std::println("widget interface BMI at:   {}", widget::interface_dialect());
  std::println("widget objects built at:   {}", widget::compiled_dialect());
  std::println("{}", widget::describe(5));

  auto response = cpr::Get(cpr::Url{"https://mesonbuild.com/Release-notes.html"});
  std::println("cpr worked! HTTP code {}", response.status_code);

  do_a_printf();
  return 0;
}
