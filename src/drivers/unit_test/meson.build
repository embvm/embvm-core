# Unit Test Driver Build Definitions

unit_test_driver_files = files(
	'driver_base.cpp',
	'gpio.cpp',
	'i2c.cpp',
	'spi.cpp'
	)

unit_test_drivers_dep = declare_dependency(
	sources: unit_test_driver_files,
	include_directories: driver_include_root
)
