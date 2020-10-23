project('gsl-lite',
	version: '0.37.1'
)

gsl_lite_dep = declare_dependency(
	include_directories: include_directories('include'),
	compile_args: [
		'-Dgsl_CONFIG_DEFAULTS_VERSION=1',
		'-Dgsl_CONFIG_NOT_NULL_EXPLICIT_CTOR=0',
	]
)
