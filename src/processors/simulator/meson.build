# meson build for Simulator Processor

simulator_processor_dep = declare_dependency(
	include_directories: [
		include_directories('.'),
	],
	sources: files('simulator_processor.cpp'),
	link_args: '-e_entry',
	# TODO: is an include dependnecy needed here?
)
