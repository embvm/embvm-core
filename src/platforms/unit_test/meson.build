# Unit testing platform definition

# The unit test platform does not rely on our libc or libc++ because we want it to
# always compile for the native machine. Disabling exceptions or STDIO would mean
# that the tests fail to compile.
unit_test_platform_dep = declare_dependency(
	include_directories: include_directories('.'),
	sources: files('boot.cpp'),
	dependencies: [
		unit_test_hw_platform_dep,
		posix_os_dep,
	]
)
