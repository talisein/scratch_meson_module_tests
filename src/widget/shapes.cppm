// Interface partition: just another .cppm of the same target, no declaration
// needed. It imports fmt, so importers of widget need fmt's BMI too.
export module widget:shapes;

// Re-exported: the primary gets these through us. gcc 16 ICEs (module.cc
// write_location) if a primary both `export import`s a partition and imports a
// header unit that the partition imports too.
export import <string>;
export import <string_view>;
import fmt;

export namespace widget {

struct shape {
  std::string_view name;
  int sides;
};

std::string render(const shape& s) {
  return fmt::format("{} ({} sides)", s.name, s.sides);
}

}  // namespace widget
