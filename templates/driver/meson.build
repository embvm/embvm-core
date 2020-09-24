# meson build for Driver

# To get this into the build, go up one level in the directory tree and add
# a `subdir(your_hw_platform_dir)` command to the subdir group

# Drivers should be typically build as dependencies and added into the HW Platform or Platform
# dependency list. If the driver has items (such as include paths) which should not be leaked into
# other layers, then build the drier as a static library. Create a dependency which includes
# safe include paths and links against the library. The platform will still reference the dependency.

# This dependency is meant to be used by platforms built with this driver
template_driver_dep = declare_dependency(
	include_directories: include_directories('.'),
	sources: [files('your_driver.cpp')],
	dependencies: [
		# Put build dependencies for your processor and drivers here
	]
)

# This should be added to the containing build file's test file list
template_driver_test_files = files('template_driver_tests.cpp')
