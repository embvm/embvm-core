APP_sources = [
	'main.c'
]

APP = executable('APP',
	APP_sources,
	dependencies: [
		utility_dep,
		libexample_dep,
	],
	install: false,
)

executable('APP_native',
	APP_sources,
	dependencies: [
		utility_native_dep,
		libexample_native_dep,
	],
	install: false,
	native: true
)

#############################
# Output Conversion Targets #
#############################

APP_hex = custom_target('APP.hex',
	input: APP,
	output: 'APP.hex',
	command: host_hex_conversion,
	build_by_default: true
)

APP_hex = custom_target('APP.bin',
	input: APP,
	output: 'APP.bin',
	command: host_bin_conversion,
	build_by_default: true
)

