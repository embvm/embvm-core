# src/ Meson Build File

###############################
# Internal Include Dependency #
###############################
# Force into an array so we can concatenate
# TODO Also, what do we do about framework_include_dep and farmework_host_inculde_dep?
# Ideally I want these combined so that they are in the proper order for users,
# instead of requiring the users to deal with placing them in the proper order!!!
# Maybe there's a "core" dep which can be used without the host/include for the
# hosted version, and then "host_include_dep" and "native_include_dep" just include "framework_include_dep"
# TODO Also - do we need a libc_header_include_dep so we can enforce proper order?
framework_include_dep = []
framework_include_dep += declare_dependency(
	include_directories: [
	  libcxx_extensions_include_dir,
	  include_directories('utilities'),
	  include_directories('subsystems'),
	  include_directories('etl'),
	],
	dependencies: [
		etl_dep,
		gsl_lite_dep
	]
)

# This should be used for building host machine targets (native:false)
framework_host_include_dep = declare_dependency(
	dependencies: [
		libcxx_header_include_dep,
	],
	include_directories: [
		libc_header_include,
	],
)

# This should be used for building native targets
framework_native_include_dep =  declare_dependency(
	include_directories: [
	  libc_native_header_include,
	],
	dependencies: [
		libcxx_native_header_include_dep,
	]
)

##########################
# Build Framework Pieces #
##########################

# Build Utilities, which use stdlibs
subdir('utilities')

# Build the framework core
subdir('core')

subdir('subsystems')

subdir('drivers')

subdir ('os')

# Pick up processor definitions (used by hw platforms)
subdir('processors')

# Pick up HW platform defintions (used by platforms)
subdir('hw_platforms')

# Pick up platform definitions
subdir('platforms')
