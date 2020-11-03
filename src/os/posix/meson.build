# Posix OS Build Files

posix_os_files = files(
	'posix_os.cpp',
	'libcpp_threading.cpp',
)

posix_test_files = files(
	'posix_tests.cpp',
)

posix_dep = dependency('threads')
posix_os_link_args = []

if (build_machine.system() == 'linux')
	posix_os_link_args += ['-lrt', '-lpthread']
endif

posix_os_dep = declare_dependency(
	include_directories: include_directories('.'),
	sources: posix_os_files,
	link_args: posix_os_link_args,
	dependencies:[
		posix_dep,
		framework_include_dep
	]
)

######################
# Supporting Tooling #
######################

catch2_tests_dep += declare_dependency(
	sources: posix_test_files
)
