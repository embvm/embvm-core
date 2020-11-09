# Template Hardware Platform Build Definition

# This dependency is meant to be used by platforms built using this HW
template_hw_platform_dep = declare_dependency(
	include_directories: include_directories('.'),
	sources: [
		files('template_hw_platform.cpp')
	],
	dependencies: [
		# Put build dependencies for your processor and drivers here
	]
)

catch2_tests_dep += declare_dependency(
	sources: files('template_hw_platform_tests.cpp'),
	dependencies: template_hw_platform_dep
)
