// A private module: importable only by this target's own sources, never
// published to anything that links widget, and its BMI stays out of the shared
// cache. Another target is free to have its own module called widget_secret.
module;

#include "dialect.h"

export module widget_secret;

import <string>;
import fmt;

export namespace widget_secret {

// Only the implementation unit imports this, so it is only ever compiled in the
// library's own C++20 flag class.
std::string stamp() {
  return fmt::format("[secret: sealed at dialect {}]", WIDGET_DIALECT);
}

}  // namespace widget_secret
