# utilities/ Master Meson Build File

framework_include_dep += declare_dependency(
	compile_args: '-DDEBUG_ASSERT_NO_STDIO',
	include_directories: [
		include_directories('type_safe/include', is_system: true),
		# Unfortunately, type_safe directly includes debug_assert.hpp - let's make that work
		include_directories('debug_assert', is_system: true)
	]
)

subdir('active_object')
subdir('aligned_ptr')
subdir('bits')
subdir('bounce')
subdir('dispatch')
subdir('endian')
subdir('function_queue')
subdir('instance_list')
subdir('interrupt_condition')
subdir('interrupt_lock')
subdir('nop_lock')
subdir('sbrm')
subdir('time')
subdir('volatile')

utilities_sources = [
	function_queue_files,
	nop_lock_files
]

## TODO: clean up the way libc/libcxx dependencies are handled?

libutil_native = static_library(
	'util_native',
	sources: [utilities_sources],
	dependencies: [
		framework_include_dep,
		framework_native_include_dep
	],
	native: true,
	build_by_default: meson.is_subproject() == false
)

libutil = static_library(
	'util',
	sources: [utilities_sources],
	dependencies: [
		framework_include_dep,
		framework_host_include_dep
	],
	build_by_default: meson.is_subproject() == false
)

######################
# Supporting Tooling #
######################

clangtidy_files += utilities_sources
catch2_tests_dep += declare_dependency(
	sources: [
		active_object_test_files,
		aligned_ptr_test_files,
		bits_test_files,
		bounce_test_files,
		dispatch_test_files,
		endian_test_files,
		function_queue_test_files,
		instance_list_test_files,
		interrupt_condition_test_files,
		interrupt_lock_test_files,
		sbrm_test_files,
		time_test_files,
		volatile_test_files
	]
)
