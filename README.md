# Meson Module Playground
This repo exercises my meson [named modules implementation](https://github.com/talisein/meson/tree/bmi-classes)

That meson modules implementation and this demo work with gcc, clang, and msvc.

| Feature | notes |
| --- | --- |
| Modules | (import foo; export module bar;) |
| partitions, implementation units, private modules | `src/widget` uses the lot: primary interface, interface partition, internal partition, implementation unit, and a private module |
| private by construction | `test-alpha` and `test-beta` both `export module test;` with different bodies -- legal, because a module an executable provides is never published |
| generated modules | a `custom_target` writes the module interface; meson only knows its output name at setup |
| import std; | Your meson.build needs `dependency('std')` or `dependency('std-nothreads')` |
| header units, user form | `hello` declares `cpp_header_units: ['cpr/cpr.h']` and writes `import "cpr/cpr.h";`; cpr itself knows nothing |
| header units, system form | `widget` (C++20) and the test exes (C++23) declare `<string>`, `<string_view>`, `<vector>` instead of importing std |
| BMI flag classes | `src/widget` is C++20, `hello` is C++23; meson builds one BMI per flag class, objects still build once |

The build pulls in:

| subproject | what it demonstrates |
| --- | --- |
| [fmt](https://github.com/fmtlib/fmt), upstream, with a simplified `subprojects/packagefiles/fmt/meson.build` | a library whose only source is a module interface, built with `FMT_IMPORT_STD` |
| `tools/gen-module.py` (in-tree) | a generated module interface that does `import std;` and `import fmt;` and exports `greeting` |
| [a simple library that imports fmt](https://github.com/talisein/subproject_using_fmt.git) | a subproject module importing another subproject's module |
| [cpr](https://github.com/libcpr/cpr) (a c++ curl wrapper), upstream, with a simplified `subprojects/packagefiles/cpr/meson.build` | a plain library, no modules anywhere in it; the `hello` executable declares `cpp_header_units: ['cpr/cpr.h']` and writes `import "cpr/cpr.h";` |
| curl, from wrapdb | only used where there is no system libcurl (i.e. Windows) |

## BMI flag classes

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
meson compiles the interfaces once per flag class and links the objects once.
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

The header units split the same way. gcc mangles a relative header-unit name into
`gcm.cache/,/`, and each class gets its own alias root under it, so the same
`<vector>` is built once per class:

```
$ ls build/gcm.cache/,/meson-private/imap/*/
4ddb75596750/  string string_view vector          # C++20 (widget)
3d7f99f98590/  string string_view vector print    # C++23 (test-alpha, test-beta)
08bd81e2479f/  string string_view vector          # C++23 +WITH_GZFILEOP
80c0864fc068/  subprojects/cpr/include/cpr/cpr.h  # the user-form unit hello declares
```

The private module never appears anywhere in the cache -- it lives in
`build/meson-private/<target>@bmi-private/widget_secret.gcm`, and only widget's
own implementation unit can import it.

Two things to know before you declare a std header as a unit:

- **Don't mix it with `import std;` in one target.** clang has no search path for
  header units, so meson passes `-fmodule-file=` for a target's units to every TU
  in it; a sibling TU that imported std would see e.g. `std::string_view` declared
  by the `<string>` unit but unreachable. Per target, pick one or the other. That
  is why nothing in `widget` or the test exes says `import std;`.
- **gcc 16 ICEs** (`write_location, at cp/module.cc:18804`) if a primary interface
  both `export import`s a partition and imports a header unit the partition also
  imports. `shapes.cppm` therefore does `export import <string>;` and the primary
  takes them through the partition.

## Two modules with the same name

`src/test.cppm` is `export module test;` -- a registry with `register_test()` and
`run()`. It is a source of **both** the test-alpha and test-beta executables, so
one module name has two providers in this build. `src/alpha.cppm` and
`src/beta.cppm` each import it and register their own tests, and the mains are
just:

```cpp
import test;
import alpha;   // registering is a side effect of the module initializer
int main() { return test::run(); }
```

so `test::run()` does something different in each executable. Nothing can link an
executable, so a module one provides is private by construction: the name is
never published, and each target gets its own BMI, away from the shared cache.

```
$ find build -path '*bmi-private*'
build/meson-private/test-alpha@exe@bmi-private/test.gcm    <- module test, alpha's copy
build/meson-private/test-alpha@exe@bmi-private/alpha.gcm
build/meson-private/test-beta@exe@bmi-private/test.gcm     <- module test, beta's copy
build/meson-private/test-beta@exe@bmi-private/beta.gcm
build/meson-private/widget@sha@bmi-private/widget_secret.gcm
```

Two *libraries* exporting one module name is a hard error instead -- one public
owner per name per machine.

`meson test` runs both:

```
1/2 scratch-meson-module-tests:alpha OK
2/2 scratch-meson-module-tests:beta  OK
```

## gcc 16, 15. 14 gracefully fails due to no import std.
`python path-to-talisein-meson/meson.py setup build -Dcpp_std=c++23`

## clang 22, 21, 20. 19 fails gracefully due to no import std.
`CXX=clang++ python path-to-talisein-meson/meson.py setup build-clang -Dcpp_std=c++23`

Both compilers, all green, in these containers:

| base | gcc | clang |
| --- | --- | --- |
| fedora:44 | 16.1.1 OK | 22.1.8 OK |
| fedora:43 | 15.2.1 OK | 21.1.8 OK |
| fedora:42 | 15.2.1 OK | 20.1.8 OK |
| debian:sid | 15.3.0 OK | 21.1.8 OK |

## msvc
`import std;` wants `c++latest` (meson's msvc map has no `c++23`), so
`meson setup build -Dcpp_std=c++latest`. The widget target overrides itself to
`c++20` either way, which is what makes the flag classes appear. cpr's libcurl
comes from the wrapdb curl wrap where there is no system one, and curl picks up
schannel on Windows, so nothing else needs installing.


# Run in a container
```bash
podman build -t modtest-f44 --build-arg BASE_IMAGE=fedora:44 .
podman run --rm modtest-f44
```
