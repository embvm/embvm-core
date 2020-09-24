# core/ Master Meson Build File

subdir('boot')
subdir('driver')
subdir('hw_platform')
subdir('platform')
subdir('register')
subdir('rtos')

incs_core = include_directories('.')

core_sources = [
	driver_files,
	platform_files,
]

threadless_core_sources = core_sources

core_test_files = [
	boot_test_files,
	driver_test_files,
	hw_platform_test_files,
	platform_test_files,
	register_test_files,
	rtos_test_files
]

if enable_threading
	core_sources += rtos_files
else
	# The Tests still need rtos_files
	core_test_files += rtos_files
endif

framework_lint_files += core_sources

###########################
# Libcore Library Targets #
###########################

libcore = static_library(
	'core',
	sources: [core_sources],
	cpp_args: framework_cpp_args,
	dependencies: [libcxx_header_include_dep],
	include_directories: [
		libc_header_include,
		incs_core,
		inc_framework
	],
)

libcore_threadless = static_library(
	'core_threadless',
	sources: [core_sources],
	cpp_args: framework_cpp_args,
	dependencies: [libcxx_header_include_dep],
	include_directories: [
		libc_header_include,
		incs_core,
		inc_framework
	],
)

libcore_native = static_library(
	'core_native',
	sources: [core_sources],
	cpp_args: framework_cpp_args,
	dependencies: [libcxx_native_header_include_dep],
	include_directories: [
		libc_header_include,
		incs_core,
		inc_framework
	],
	native: true
)

libcore_threadless_native = static_library(
	'core_threadless_native',
	sources: [core_sources],
	cpp_args: framework_cpp_args,
	dependencies: [libcxx_native_header_include_dep],
	include_directories: [
		libc_header_include,
		incs_core,
		inc_framework
	],
	native: true
)

# Use system libc/libcpp
libcore_hosted_native = static_library(
	'core_hosted_native',
	sources: [core_sources],
	cpp_args: framework_cpp_args,
	include_directories: [incs_core, inc_framework],
	native: true
)

# Use system libc/libcpp
libcore_threadless_hosted_native = static_library(
	'core_threadless_hosted_native',
	sources: [core_sources],
	cpp_args: framework_cpp_args,
	include_directories: [incs_core, inc_framework],
	native: true
)

########################
# Libcore Dependencies #
########################

libcore_dep = declare_dependency(
	link_with: libcore,
	sources: [
		driver_platform_files,
		boot_platform_files
	],
	include_directories: [incs_core, inc_framework]
)

libcore_native_dep = declare_dependency(
	link_with: libcore_native,
	sources: [
		driver_platform_files,
		boot_platform_files
	],
	include_directories: [incs_core, inc_framework]
)

libcore_threadless_dep = declare_dependency(
	link_with: libcore_threadless,
	sources: [
		driver_platform_files,
		boot_platform_files,
	],
	compile_args: ['-fno-threadsafe-statics'],
	include_directories: [incs_core, inc_framework]
)

libcore_threadless_native_dep = declare_dependency(
	link_with: libcore_threadless_native,
	sources: [
		driver_platform_files,
		boot_platform_files
	],
	compile_args: ['-fno-threadsafe-statics'],
	include_directories: [incs_core, inc_framework]
)
