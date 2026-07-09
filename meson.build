project(
  'scratch-meson-module-tests',
  'cpp',
  version: '0.0.1',
  meson_version: '>=0.50.0',
  license: 'MIT WITH fmt-exception',
  default_options: ['cpp_std=c++23']
)

fmt_module_dep = dependency('fmt-module', fallback: ['fmt'])
fmt_using_library_dep = dependency('fmt-using-library', fallback: ['subproject_using_fmt'])
cpr_has_header_unit_dep = dependency('cpr-as-header-unit', fallback: ['cpr_has_header_unit'])

######## peel gio stuff #########
# Peel source generation for gio. Very much a prototype, gio has a lot of
# expose-global-module-tu-local warnings, but sufficient for our demo purposes
gio_dep = dependency('gio-2.0')
peel_subproj = subproject('peel')
peel_dep = peel_subproj.get_variable('peel_dep')
peel_gen = find_program('peel-gen')
peel_gio = custom_target('peel-codegen',
  command: [peel_gen, '--recursive', '--emit-modules', '--out-dir', '@OUTDIR@', 'Gio', '2.0'],
  output: 'peel',
)

cp = find_program('cp')
gio_cppm = custom_target('gio-cppm',
  command: [cp, meson.current_build_dir() / 'peel' / 'Gio' / 'Gio.cppm', '@OUTPUT@'],
  output: 'Gio.cppm',
  depends: peel_gio,
)

gio_module_lib = library('gio-module', gio_cppm,
  cpp_modules: true,
  dependencies: [gio_dep, peel_dep],
)

gio_module_dep = declare_dependency(
  link_with: gio_module_lib,
  dependencies: [gio_dep, peel_dep],
)
######## end peel gio stuff #########

std_dep = dependency('std')
exe = executable('hello', 'src/main.cpp',
                 dependencies: [gio_module_dep, std_dep, fmt_using_library_dep, cpr_has_header_unit_dep])
