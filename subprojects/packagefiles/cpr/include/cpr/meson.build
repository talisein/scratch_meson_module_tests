ver = meson.project_version().split('.')
cprver_conf = configuration_data()
cprver_conf.set('cpr_VERSION', meson.project_version())
cprver_conf.set('cpr_VERSION_MAJOR', ver[0])
cprver_conf.set('cpr_VERSION_MINOR', ver[1])
cprver_conf.set('cpr_VERSION_PATCH', ver[2])
cprver_conf.set('cpr_VERSION_NUM',
  (ver[0].to_int() * 65536 + ver[1].to_int() * 256 + ver[2].to_int()).to_string())

# Lands next to the shipped headers in the build tree, so cpr/cpr.h's
# #include "cpr/cprver.h" resolves -- and it exists at setup time, which is what
# a GCC header unit needs.
configure_file(
  input: '../../cmake/cprver.h.in',
  output: 'cprver.h',
  format: 'cmake',
  configuration: cprver_conf,
)
