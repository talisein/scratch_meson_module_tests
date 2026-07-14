# Meson Module Playground
This repo exercises my meson [named modules implementation](https://github.com/talisein/meson/tree/bmi-classes)

That meson modules implementation and this demo work with gcc, clang, and msvc.

| Feature | notes |
| --- | --- |
| Modules | `import foo;` `export module bar;` |
| partitions, implementation units, private modules | `export module widget:shapes;` `module widget:detail;` |
| private by construction | `export module unittests;` in multiple executables |
| generated modules | `import foo; export module foo:bar;` works in generated sources |
| import std; | Use `dependency('std')` or `dependency('std-nothreads')` |
| header units | `import "cpr/cpr.h";` `import <vector>;` |
| BMI classes | safe C++20 vs C++23, -freflection, -DFOO variance across targets. |

## BMI classes

`src/widget` builds at C++20 (`override_options: ['cpp_std=c++20']`)

```meson
library('widget',
  'src/widget/widget.cppm', 'src/widget/shapes.cppm', 'src/widget/secret.cppm',
  'src/widget/detail.cpp', 'src/widget/impl.cpp',
  cpp_module_interfaces: ['src/widget/widget.cppm', 'src/widget/shapes.cppm'],
  cpp_internal_partitions: ['src/widget/detail.cpp'],
  cpp_private_module_interfaces: ['src/widget/secret.cppm'],
  cpp_header_units: ['<string>', '<string_view>', '<vector>'],
  override_options: ['cpp_std=c++20'],
  dependencies: [std_dep, fmt_dep])
```

`hello` is C++23 and imports it. A C++20 BMI is not loadable from a C++23 TU, so
meson compiles the interfaces once per class and links the objects once.
`hello` prints the proof:

```
main compiled at:          202302
widget interface BMI at:   202302   <- widget's interface, re-BMI'd for C++23
widget objects built at:   202002   <- widget's objects, compiled once at C++20
pentagon (5 sides) [secret: sealed at dialect 202002]
```

The cache shows the same thing. Four classes fall out of this build: C++20 vs
C++23, fmt's own `-DFMT_IMPORT_STD`, and `-DWITH_GZFILEOP` that libcurl's
pkg-config hands to whoever links cpr.

```
$ ls build/gcm.cache/*/
13f428f77524/  fmt std                                              # C++23 +FMT_IMPORT_STD (fmt)
41266af1e1e8/  fmt std std.compat greeting fmt_using_library
               widget widget-shapes                                 # C++23 (greeting, test exes)
c30257d66a32/  fmt std std.compat greeting fmt_using_library
               widget widget-shapes                                 # C++23 +WITH_GZFILEOP (hello)
f972e2c81ee4/  fmt std widget widget-shapes widget-detail           # C++20 (widget itself)
```

The header units split the same way.

```
$ ls build/gcm.cache/,/meson-private/imap/*/
4ddb75596750/  string string_view vector          # C++20 (widget)
3d7f99f98590/  string string_view vector print    # C++23 (test-alpha, test-beta)
08bd81e2479f/  string string_view vector          # C++23 +WITH_GZFILEOP
80c0864fc068/  subprojects/cpr/include/cpr/cpr.h  # the user-form unit hello declares
```

## Two modules with the same name

`src/test.cppm` is `export module test;` -- a registry with `register_test()`
and `run()`. Two executables alpha and beta use it, so two 'test' named modules
are exported in one meson build, but for different targets.

```
$ find build -path '*bmi-private*'
build/meson-private/test-alpha@exe@bmi-private/test.gcm    <- module test, alpha's copy
build/meson-private/test-alpha@exe@bmi-private/alpha.gcm
build/meson-private/test-beta@exe@bmi-private/test.gcm     <- module test, beta's copy
build/meson-private/test-beta@exe@bmi-private/beta.gcm
build/meson-private/widget@sha@bmi-private/widget_secret.gcm
```

## working compiler versions

gcc 16, 15. 14 gracefully fails due to no import std.
`python path-to-talisein-meson/meson.py setup build -Dcpp_std=c++23`

clang 22, 21, 20. 19 fails gracefully due to no import std.
`CXX=clang++ python path-to-talisein-meson/meson.py setup build-clang -Dcpp_std=c++23`

For msvc, `meson setup build -Dcpp_std=c++latest --default-library static`.

## Run in a container

```bash
podman build -t modtest-f44 --build-arg BASE_IMAGE=fedora:44 .
podman run --rm modtest-f44
```
