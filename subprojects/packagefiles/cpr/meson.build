project(
  'cpr',
  'cpp',
  version: '1.14.2',
  meson_version: '>=1.12.0',
  license: 'MIT',
  default_options: ['cpp_std=c++17'],
)

# Upstream cpr is a CMake project; this is a minimal stand-in that builds the
# ordinary library and nothing else. Nothing here knows about modules -- the
# executable in the root project imports cpr/cpr.h as a header unit on its own.

curl_dep = dependency('libcurl')
threads_dep = dependency('threads')

# cpr/cpr.h includes cpr/cprver.h, which upstream generates with CMake. Without
# it the build would silently pick up a system-installed cpr's copy.
subdir('include/cpr')

cpr_sources = files(
  'cpr/accept_encoding.cpp',
  'cpr/async.cpp',
  'cpr/auth.cpp',
  'cpr/callback.cpp',
  'cpr/cert_info.cpp',
  'cpr/connection_pool.cpp',
  'cpr/cookies.cpp',
  'cpr/cprtypes.cpp',
  'cpr/curl_container.cpp',
  'cpr/curlholder.cpp',
  'cpr/curlmultiholder.cpp',
  'cpr/error.cpp',
  'cpr/file.cpp',
  'cpr/interceptor.cpp',
  'cpr/multipart.cpp',
  'cpr/multiperform.cpp',
  'cpr/parameters.cpp',
  'cpr/payload.cpp',
  'cpr/proxies.cpp',
  'cpr/proxyauth.cpp',
  'cpr/redirect.cpp',
  'cpr/response.cpp',
  'cpr/session.cpp',
  'cpr/sse.cpp',
  'cpr/ssl_ctx.cpp',
  'cpr/threadpool.cpp',
  'cpr/timeout.cpp',
  'cpr/unix_socket.cpp',
  'cpr/util.cpp',
)

cpr_lib = library(
  'cpr',
  cpr_sources,
  include_directories: 'include',
  dependencies: [curl_dep, threads_dep],
)

cpr_dep = declare_dependency(
  include_directories: 'include',
  link_with: cpr_lib,
  dependencies: [curl_dep, threads_dep],
)

meson.override_dependency('cpr', cpr_dep)
