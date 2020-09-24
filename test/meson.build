# Test Build Definition

PROJECT_test_files = files(
	'main.c',
	'test_suite.c',
)

clangtidy_files += PROJECT_test_files

catch2_tests_dep += declare_dependency(
	sources: 'catch2_test_case.cpp',
)

#######################
# Test Compiler Flags #
#######################

test_suite_compiler_flags = native_c_compiler.get_supported_arguments(
	'-Wno-unused-parameter',
)

###############
# Test Target #
###############

PROJECT_tests = executable('PROJECT_tests',
	PROJECT_test_files,
	dependencies: [
		cmocka_native_dep,
	],
	link_args: native_map_file.format(meson.current_build_dir() + '/PROJECT_tests'),
	c_args: test_suite_compiler_flags,
	native: true
)

#############################
# Register Tests with Meson #
#############################

# CMocka Test Suite #

test('PROJECT_tests',
	PROJECT_tests,
	env: [cmocka_test_output_dir])

run_target('PROJECT-tests',
	command: [PROJECT_tests]
)

run_target('PROJECT-tests-xml',
	command: [
		cmocka_with_env_script,
		PROJECT_tests,
		cmocka_test_output_dir
	],
)
