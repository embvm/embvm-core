# Driver Build

driver_include_root = include_directories('.')

if enable_threading == true
	subdir('aardvark')
endif

# Technically simulator has some drivers... but we use them for unit test code.
subdir('simulator')

subdir('solomon_systech')
subdir('st')
subdir('unit_test')
