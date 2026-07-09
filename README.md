# Meson Module Playground
This repo exercises my meson [named modules implementation](https://github.com/talisein/meson/tree/talisein-cpp-header-units)

That meson modules implementation works with gcc, clang, and msvc. But this
scratch project is for gcc and clang on linux.

It implements on all 3 compilers (that have the requisite support):

| Feature | notes |
| --- | --- |
| Modules | (import foo; export module bar;) |
| import std; | Your meson.build needs `dependency('std')` or `dependency('std-nothreads')` |
| header units | Your meson.build target needs e.g. `cpp_header_units: ['foo/foo.hpp', '<vector>']` |

For clang and msvc your module interface files have to have `.cppm`
extensions. In the fmt fork below you can see I just use configure_file to copy
fmt.cc to fmt.cppm.

The build references some forks:

| playerground forks |
| --- |
| [fmt with a fmt-module meson.build](https://github.com/talisein/fmt_meson_modules.git) |
| [a simple library that imports fmt](https://github.com/talisein/subproject_using_fmt.git) |
| [cpr (a c++ curl wrapper) with a header unit meson.build](https://github.com/talisein/cpr_has_header_unit.git) |
| [peel (a c++ GObject introspection source generator) that produces module interfaces](https://gitlab.gnome.org/talisein/peel/-/tree/emit-cpp-modules?ref_type=heads) |

## gcc 16
`python path-to-talisein-meson/meson.py setup build -Dcpp_std=c++23`

## clang 22
`CXX=clang++ python path-to-talisein-meson/meson.py setup build-clang -Dcpp_std=c++23`
