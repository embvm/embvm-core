# Simulator Driver Build Definitions

simulator_driver_files = files(
	'system_clock.cpp',
	'timer.cpp'
	)

simulator_driver_dep = declare_dependency(
	sources: simulator_driver_files,
)

simulator_driver_test_files = files(
	'timer_tests.cpp',
)

######################
# Supporting Tooling #
######################
clangtidy_files += files('system_clock.cpp', 'timer.cpp')
catch2_tests_dep += declare_dependency(
	sources: simulator_driver_test_files
)
