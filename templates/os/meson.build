# meson build for Template HW platform

# To get this into the build, go up one level in the directory tree and add
# a `subdir(your_hw_platform_dir)` command to the subdir group

# This dependency is meant to be used by platforms built using this HW
template_hw_platform_dep = declare_dependency(
	include_directories: include_directories('.'),
	sources: [files('template_hw_platform.cpp')],
	dependencies: [
		# Put build dependencies for your processor and drivers here
	]
)

# This should be added to the containing build file's test file list
template_hw_platform_test_files = files('template_hw_platform_tests.cpp')
