project(
  'fmt',
  'cpp',
  version: '12.2.0',
  meson_version: '>=1.12.0',
  license: 'MIT WITH fmt-exception',
  default_options: ['cpp_std=c++20'],
)

# Upstream fmt is a CMake project; this is a deliberately minimal stand-in that
# builds only what the module demo needs: the C++20 module interface.
#
# src/fmt.cc is the whole library when built as a module -- it #includes
# format.cc and os.cc itself (hence the 'src' include dir), so there is no
# separate non-module library to build here. FMT_IMPORT_STD makes it say
# 'import std;' rather than #including the standard headers.
#
# clang and cl only accept .cppm/.ixx as a module interface, and upstream ships
# a .cc, so copy it.
fmt_cppm = configure_file(input: 'src/fmt.cc', output: 'fmt.cppm', copy: true)

fmt_lib = library(
  'fmt',
  fmt_cppm,
  cpp_args: ['-DFMT_IMPORT_STD'],
  include_directories: ['src', 'include'],
  dependencies: dependency('std'),
)

fmt_dep = declare_dependency(
  include_directories: 'include',
  link_with: fmt_lib,
)

meson.override_dependency('fmt', fmt_dep)
