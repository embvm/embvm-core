# meson build for Template platform

# To get this into the build, go up one level in the directory tree and add
# a `subdir(your_platform_dir)` command to the subdir group

# Platforms should be typically build as dependencies and added into the application dependency list
# If platforms include items (such as include paths) which should not be leaked into the main
# Application, then build the platform as a static library. Create a dependency which includes
# safe include paths and links against the library. The app will still reference the dependency.

# This dependency is meant to be used by applications built using this platform
template_platform_dep = declare_dependency(
	include_directories: include_directories('.'),
	sources: [files('platform.cpp')],
	dependencies: [
		# Put build dependencies for your HW platform here
		# Put build dependencies for your OS here
		# Put library dependencies for standard library requirements here:
		# libmemory_freelist_dep,
		# libc_dep,
		# libcxxabi_dep,
		# libcxx_full_dep,
		# compiler_rt_builtins_dep,
	],
	link_args: [
		# Does your platform have any custom link arguments, such as a linker script?
	],
)

# This should be added to the containing build file's test file list
template_platform_test_files = files('platform_tests.cpp')
