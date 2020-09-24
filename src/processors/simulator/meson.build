# meson build for Simulator Processor

simulator_processor_dep = declare_dependency(
	include_directories: [
		include_directories('.'),
		inc_framework
	],
	sources: files('simulator_processor.cpp'),
	link_args: '-e_entry',
)
