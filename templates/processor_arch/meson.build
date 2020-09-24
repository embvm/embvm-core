# meson build for Template Processor Architecture

# To get this into the build, go up one level in the directory tree and add
# a `subdir(your_hw_platform_dir)` command to the subdir group

# Architectures should be typically build as dependencies and added into the processor dependency
# list. Because the processor is compiled as a library, it's ok to pollute include paths if needed
# to compile the .cpp file.

# This dependency is meant to be added to the processor
template_arch_dep = declare_dependency(
	include_directories: include_directories('.'),
	sources: files(
		'template_processor_arch.cpp',
	)
)

# This should be added to the containing build file's test file list
template_arch_test_files = files('template_processor_arch_tests.cpp')

