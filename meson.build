project(
  'scratch-meson-module-tests',
  'cpp',
  version: '0.0.1',
  meson_version: '>=1.12.0',
  license: 'MIT WITH fmt-exception',
  default_options: ['cpp_std=c++23', 'force_fallback_for=fmt,cpr']
)

std_dep = dependency('std')
fmt_dep = dependency('fmt')
cpr_dep = dependency('cpr')
fmt_using_library_dep = dependency('fmt-using-library', fallback: ['subproject_using_fmt'])

######## generated module #########
gen_module = find_program('tools/gen-module.py')
greeting_cppm = custom_target('greeting-cppm',
  command: [gen_module, '--module', 'greeting', '--greeting', 'Hello',
            '--output', '@OUTPUT@', 'Ada', 'Alan', 'Grace'],
  output: 'greeting.cppm',
)

greeting_lib = library('greeting', greeting_cppm,
  dependencies: [std_dep, fmt_dep],
)

greeting_dep = declare_dependency(
  link_with: greeting_lib,
  dependencies: [std_dep, fmt_dep],
)
######## end generated module #########

######## fully declared module library #########
# Built on c++20
widget_lib = library('widget',
  'src/widget/widget.cppm',   # primary interface:   export module widget;
  'src/widget/shapes.cppm',   # interface partition: export module widget:shapes;
  'src/widget/secret.cppm',   # private module:      export module widget_secret;
  'src/widget/detail.cpp',    # internal partition:  module widget:detail;
  'src/widget/impl.cpp',      # implementation unit: module widget;
  cpp_module_interfaces: ['src/widget/widget.cppm', 'src/widget/shapes.cppm'], # Redundant with .cppm/.ixx extension. Required for .cc
  cpp_internal_partitions: ['src/widget/detail.cpp'],
  cpp_private_module_interfaces: ['src/widget/secret.cppm'],
  cpp_header_units: ['<string>', '<string_view>', '<vector>'], # header units at c++20 dialect
  override_options: ['cpp_std=c++20'],
  dependencies: [std_dep, fmt_dep],
)

widget_dep = declare_dependency(
  link_with: widget_lib,
  dependencies: [std_dep, fmt_dep],
)

# Use the c++20 library in a c++23 dialect executable.
# BMI classes don't just split on dialect: -D, cpp_eh, -f, etc etc. The
# implementation is an allowlist, e.g. -O -g doesn't change bmi. Anything left
# after the allowlist is hashed together.
exe = executable('hello', 'src/main.cpp', 'src/compat.cpp',
                 cpp_header_units: ['cpr/cpr.h'],
                 dependencies: [greeting_dep, widget_dep, std_dep,
                                fmt_using_library_dep, cpr_dep])

######## private executable module exports #########
# both executables export a 'test' module. The BMI do not collide.
test_alpha = executable('test-alpha',
  'src/test.cppm', 'src/alpha.cppm', 'src/alpha-main.cpp',
  cpp_module_interfaces: ['src/test.cppm', 'src/alpha.cppm'],
  cpp_header_units: ['<print>', '<string>', '<string_view>', '<vector>'], # header units at c++23 dialect
  dependencies: [greeting_dep, std_dep, fmt_dep],
)

test_beta = executable('test-beta',
  'src/test.cppm', 'src/beta.cppm', 'src/beta-main.cpp',
  cpp_module_interfaces: ['src/test.cppm', 'src/beta.cppm'],
  cpp_header_units: ['<print>', '<string>', '<string_view>', '<vector>'],
  dependencies: [widget_dep, std_dep],
)

test('alpha', test_alpha)
test('beta', test_beta)
