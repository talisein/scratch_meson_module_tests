// Internal partition: no export, and it must be a plain .cpp because cl rejects
// .ixx with /internalPartition. The cpp_internal_partitions kwarg is what makes
// cl pass that flag; it is a no-op on gcc and clang.
module widget:detail;

import <string_view>;

namespace widget::detail {

std::string_view name_for(int sides) {
  switch (sides) {
    case 3: return "triangle";
    case 4: return "square";
    case 5: return "pentagon";
    case 6: return "hexagon";
    default: return "polygon";
  }
}

}  // namespace widget::detail
