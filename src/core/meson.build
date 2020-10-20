# core/ Master Meson Build File

subdir('boot')
subdir('driver')
subdir('hw_platform')
subdir('platform')
subdir('register')
subdir('rtos')

core_sources = [
	driver_files,
	platform_files,
]

# This is added after `utilities` processing because the utils are not
# allowed to use `core` constructs.
framework_include_dep += declare_dependency(
	include_directories: include_directories('.')
)

###########################
# Libcore Library Targets #
###########################

libcore = static_library(
	'core',
	sources: [core_sources, rtos_files],
	dependencies: [
		framework_include_dep,
		framework_host_include_dep
	],
	build_by_default: meson.is_subproject() == false
)

libcore_threadless = static_library(
	'core_threadless',
	sources: [core_sources],
	dependencies: [
		framework_include_dep,
		framework_host_include_dep
	],
	build_by_default: meson.is_subproject() == false
)

libcore_native = static_library(
	'core_native',
	sources: [core_sources, rtos_files],
	dependencies: [
		framework_include_dep,
		framework_native_include_dep
	],
	native: true,
	build_by_default: meson.is_subproject() == false
)

libcore_threadless_native = static_library(
	'core_threadless_native',
	sources: [core_sources],
	dependencies: [
		framework_include_dep,
		framework_native_include_dep
	],
	native: true,
	build_by_default: meson.is_subproject() == false
)

# Use system libc/libcpp
libcore_hosted_native = static_library(
	'core_hosted_native',
	sources: [core_sources, rtos_files],
	dependencies: [
		framework_include_dep,
	],
	native: true,
	build_by_default: meson.is_subproject() == false
)

# Use system libc/libcpp
libcore_threadless_hosted_native = static_library(
	'core_threadless_hosted_native',
	sources: [core_sources,],
	dependencies: [
		framework_include_dep,
	],
	native: true,
	build_by_default: meson.is_subproject() == false
)

########################
# Libcore Dependencies #
########################

libcore_dep = declare_dependency(
	link_with: libcore,
	sources: [
		boot_platform_files
	],
	dependencies: framework_include_dep,
)

libcore_native_dep = declare_dependency(
	link_with: libcore_native,
	sources: [
		boot_platform_files
	],
	dependencies: framework_include_dep,
)

## TODO: hosted-native dependency - leave out boot_platform files perhaps?
## TODO: -fno-threadsafe-statics: check for compiler support first!
libcore_threadless_dep = declare_dependency(
	link_with: libcore_threadless,
	sources: [
		boot_platform_files,
	],
	compile_args: ['-fno-threadsafe-statics'],
	dependencies: framework_include_dep,
)

libcore_threadless_native_dep = declare_dependency(
	link_with: libcore_threadless_native,
	sources: [
		boot_platform_files
	],
	compile_args: ['-fno-threadsafe-statics'],
	dependencies: framework_include_dep,
)

######################
# Supporting Tooling #
######################

clangtidy_files += core_sources
catch2_tests_dep += declare_dependency(
	sources: [
		boot_test_files,
		driver_test_files,
		hw_platform_test_files,
		platform_test_files,
		register_test_files,
	]
)

if enable_threading == false
	# Even if threading is disabled in the VM, we need the RTOS files for tests
	catch2_tests_dep += declare_dependency(
		sources: rtos_files
	)
endif
