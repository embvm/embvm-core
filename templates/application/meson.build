skeleton_application = executable('skeleton_application',
	files('main.cpp'),
	dependencies: [
		skeleton_platform_dep,
	],
	install: false,
	link_args: host_map_file.format(meson.current_build_dir() / 'skeleton_application'),
	build_by_default: meson.is_subproject() == false
)

#############################
# Output Conversion Targets #
#############################

blinky_hex = custom_target('skeleton_application.hex',
	input: skeleton_application,
	output: 'skeleton_application.hex',
	command: host_hex_conversion,
	build_by_default: meson.is_subproject() == false
)

blinky_hex = custom_target('skeleton_application.bin',
	input: skeleton_application,
	output: 'skeleton_application.bin',
	command: host_bin_conversion,
	build_by_default: meson.is_subproject() == false
)

