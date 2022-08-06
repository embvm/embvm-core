# Posix OS Build Files

posix_os_files = files(
	'posix_os.cpp',
	'libcpp_threading.cpp',
)

posix_test_files = files(
	'posix_tests.cpp',
)

posix_dep = dependency('threads')

posix_os_dep = declare_dependency(
	include_directories: include_directories('.'),
	sources: posix_os_files,
	compile_args: '-pthread',
	link_args: '-pthread',
	dependencies:[
		#posix_dep,
		framework_include_dep
	]
)

######################
# Supporting Tooling #
######################

catch2_tests_dep += declare_dependency(
	sources: posix_test_files
)
