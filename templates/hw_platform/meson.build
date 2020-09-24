# meson build for Template HW platform

# To get this into the build, go up one level in the directory tree and add
# a `subdir(your_hw_platform_dir)` command to the subdir group

# HW Platforms should be typically build as dependencies and added into the Platform dependency
# list. If HW Platforms include items (such as include paths) which should not be leaked into the
# main Platform, then build the HW Platform as a static library. Create a dependency which includes
# safe include paths and links against the library. The Platform will still reference the dependency.

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
