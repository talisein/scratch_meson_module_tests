#!/bin/sh
# Runs the gcc and clang builds back to back without stopping on failure,
# so every compiler's error output is visible in a single container run.
# Any arguments are forwarded to both `meson setup` invocations.

echo "==================== toolchain versions ===================="
. /etc/os-release && echo "distro: $PRETTY_NAME"
gcc --version | head -n1
clang --version | head -n1
echo "meson: $(meson --version)"
echo

gcc_rc=1
echo "==================== gcc: meson setup build ===================="
if meson setup build "$@" -Dcpp_std=c++23; then
    echo "==================== gcc: ninja -C build ===================="
    ninja -C build && build/hello && gcc_rc=0
fi

clang_rc=1
echo "==================== clang: meson setup build-clang ===================="
if CC=clang CXX=clang++ meson setup build-clang "$@" -Dcpp_std=c++23; then
    echo "==================== clang: ninja -C build-clang ===================="
    ninja -C build-clang && build-clang/hello && clang_rc=0
fi

echo
echo "==================== summary ===================="
[ "$gcc_rc" -eq 0 ] && echo "gcc:   OK" || echo "gcc:   FAILED"
[ "$clang_rc" -eq 0 ] && echo "clang: OK" || echo "clang: FAILED"

exit $((gcc_rc + clang_rc))
