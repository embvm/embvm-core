# Specify Settings for Embedded VM Core Test Build
embvm_test_dep = declare_dependency(
	compile_args: [
		#'-DCATCH_CONFIG_DISABLE',
		'-DCATCH_CONFIG_FAST_COMPILE',
		'-DCATCH_CONFIG_NO_WCHAR',
		'-DCATCH_CONFIG_NO_POSIX_SIGNALS',
		'-DCATCH_CONFIG_DISABLE_MATCHERS',
		'-DMSE_DISABLE_IOSTREAM',
		'-DMSE_SCOPEPOINTER_DEBUG_RUNTIME_CHECKS_DISABLED',
		'-fexceptions',
		'-funwind-tables',
	],
	sources: files('putchar.c'),
	# TODO: use dependencies to simplify all of this linker stuff
	link_with: [
		libcore_hosted_native,
		libutil_native
	],
	dependencies: [
		unit_test_platform_dep,
		printf_dep,
		libmemory_hosted_native_dep,
		framework_include_dep
	],
)

catch2_tests_dep += embvm_test_dep
