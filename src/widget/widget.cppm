// Primary module interface.
module;

#include "dialect.h"

export module widget;

export import :shapes;


export namespace widget {

// The dialect macro is expanded when this interface is compiled -- and with BMI
// flag classes it is compiled once per class, so this reports the dialect of
// whoever imports it, not the one this library builds its objects with.
constexpr long interface_dialect() { return WIDGET_DIALECT; }

// Defined in the implementation unit, which is compiled exactly once, at this
// library's own C++20.
long compiled_dialect();

std::string describe(int sides);

}  // namespace widget
