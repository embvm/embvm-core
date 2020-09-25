# Logging Subsystem Build File

logging_subsystem_dep = declare_dependency(
	include_directories: include_directories('.'),
)

logging_test_files = files(
	'logging_tests.cpp',
)

######################
# Supporting Tooling #
######################
# TODO: clangtidy_files +=
catch2_tests_dep += declare_dependency(
	sources: logging_test_files
)
