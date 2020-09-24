# Test Meson Build File

test_app_files = [
	'catch/catch_main.cpp'
]

test_settings = [
	'-DCATCH_CONFIG_FAST_COMPILE',
	'-DCATCH_CONFIG_NO_WCHAR',
	'-DCATCH_CONFIG_NO_POSIX_SIGNALS',
	'-DCATCH_CONFIG_DISABLE_MATCHERS',
	'-DMSE_DISABLE_IOSTREAM',
	'-DMSE_SCOPEPOINTER_DEBUG_RUNTIME_CHECKS_DISABLED',
	'-fexceptions',
	'-funwind-tables',
]

framework_tests = executable('framework_tests',
	[test_app_files, framework_test_files],
	link_with: [libcore_hosted_native, libutil_native],
	include_directories: [inc_framework, libcxx_extensions_include_dir],
	cpp_args: test_settings,
	dependencies: [unit_test_platform_dep, printf_dep, libmemory_hosted_native_dep],
	native: true
)

test('framework_tests',
	framework_tests,
	args: ['-s', '-r', 'junit', '-o', meson.build_root() + '/framework_tests.xml'],
	is_parallel : false)

run_target('framework-tests',
	command: [framework_tests])

run_target('framework-tests-xml',
	command: [framework_tests, '-s', '-r', 'junit', '-o', meson.build_root() + '/framework_tests.xml'])

run_target('framework-tests-catch-xml',
	command: [framework_tests, '-s', '-r', 'xml', '-o', meson.build_root() + '/framework_tests_catch.xml'])
