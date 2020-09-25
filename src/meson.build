# src/ Meson Build File

framework_includes = [
  ## TODO: use dependency
  include_directories('../subprojects/etl/include', is_system: true),
  ## TODO: use dependency
  include_directories('../subprojects/GSL-3.1.0/include', is_system: true),
  ## TODO: Use dependency
  libcxx_extensions_include_dir,
  include_directories('utilities'),
  include_directories('subsystems'),
  include_directories('etl')
]

framework_cpp_args = ['-DCATCH_CONFIG_DISABLE', '-DDEBUG_ASSERT_NO_STDIO']

##########################
# Build Framework Pieces #
##########################

# Build Utilities, which use stdlibs
subdir('utilities')

# Build the framework core
subdir('core')

# TODO: is this actually needed, since we now have a libcore dependency?
framework_includes += core_includes

subdir('subsystems')

subdir('drivers')

subdir ('os')

# Pick up processor definitions (used by hw platforms)
subdir('processors')

# Pick up HW platform defintions (used by platforms)
subdir('hw_platforms')

# Pick up platform definitions
subdir('platforms')
