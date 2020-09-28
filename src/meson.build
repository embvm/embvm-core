# src/ Meson Build File

###############################
# Internal Include Dependency #
###############################
# Force into an array so we can concatenate
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
		microsoft_gsl_dep
	]
)

# This should be used for building host machine targets (native:false)
framework_host_include_dep = declare_dependency(
	include_directories: [
		libc_header_include,
	],
	dependencies: [
		libcxx_header_include_dep,
	]
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
