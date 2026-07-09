# Test container for the talisein-cpp-header-units meson branch.
#
# The image installs toolchains + deps and the meson branch; the builds run
# when you start the container, so builds that fail (old compilers) still
# produce a runnable image and you can read the error output.
#
# Usage:
#   podman build -t modtest-f43 --build-arg BASE_IMAGE=fedora:43 .
#   podman run --rm modtest-f43            # network needed for git wraps
#
# Other bases:
#   --build-arg BASE_IMAGE=fedora:42   (41, 40, 39, ...)
#   --build-arg BASE_IMAGE=ubuntu:25.04   (24.04, 22.04, ...)
#
# Optional extras, e.g. a newer clang on Ubuntu:
#   --build-arg EXTRA_PKGS="clang-20 clang-tools-20"

ARG BASE_IMAGE=fedora:43
FROM ${BASE_IMAGE}

ARG MESON_REPO=https://github.com/talisein/meson.git
ARG MESON_BRANCH=talisein-cpp-header-units
ARG EXTRA_PKGS=""

# Distro-aware dependency install. glib2/gio + gobject-introspection cover
# peel-gen's GIR input; libcurl is for the cpr header-unit subproject.
# clang-tools{,-extra} provides clang-scan-deps for module dep scanning.
RUN . /etc/os-release; \
    case "$ID" in \
    fedora) \
        dnf install -y \
            git python3 ninja-build pkgconf-pkg-config \
            gcc gcc-c++ libstdc++-devel \
            clang clang-tools-extra \
            glib2-devel gobject-introspection-devel \
            libcurl-devel \
            ${EXTRA_PKGS} \
        && dnf clean all \
        ;; \
    ubuntu|debian) \
        apt-get update \
        && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
            git ca-certificates python3 ninja-build pkg-config \
            gcc g++ \
            clang clang-tools \
            libglib2.0-dev libgirepository1.0-dev \
            libcurl4-openssl-dev \
            ${EXTRA_PKGS} \
        && rm -rf /var/lib/apt/lists/* \
        ;; \
    *) echo "Unsupported distro: $ID" >&2; exit 1 ;; \
    esac

RUN git clone --depth 1 --branch ${MESON_BRANCH} ${MESON_REPO} /opt/meson \
    && ln -s /opt/meson/meson.py /usr/local/bin/meson \
    && meson --version

WORKDIR /work
COPY . /work
RUN chmod +x /work/run-builds.sh

# Extra args are passed to both `meson setup` invocations.
ENTRYPOINT ["/work/run-builds.sh"]
