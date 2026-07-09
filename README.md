# Meson Module Playground
This repo exercises my meson [named modules implementation](https://github.com/talisein/meson/tree/talisein-cpp-header-units)

## gcc 16
`python path-to-talisein-meson/meson.py setup build -Dcpp_std=c++23`

## clang 22
`CXX=clang++ python path-to-talisein-meson/meson.py setup build-clang -Dcpp_std=c++23`
