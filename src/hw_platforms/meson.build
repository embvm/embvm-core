# meson build for framework internal HW Platform definitions

subdir('unit_test')

if enable_threading == true
	subdir('fwdemo_simulator')
	subdir('blinky_simulator')
endif

if target_architecture == 'arm' and target_compiler_id == 'gcc'
subdir('nordic')
endif
