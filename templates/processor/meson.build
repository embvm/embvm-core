# meson build for Template Processor

# To get this into the build, go up one level in the directory tree and add
# a `subdir(your_processor_dir)` command to the subdir group

# Processor impelmentations require items (such as include paths) which should not be leaked into
# other layers. Build the processor as a static library. Create a dependency which includes
# safe include paths and links against the library. The hardware platform will reference the dep.

# This should be added to the containing build file's test file list
template_processor_test_files = files('template_processor_tests.cpp')

template_processor = static_library('template_processor',
    [
    	'template_processor.cpp',
    	# Include other files you might need:
    	# '../mdk/gcc_startup_nrf52840.S',
    	# '../mdk/system_nrf52840.c',
    ],
    include_directories: [
    	# These include directories are for building the library. You can safely
    	# include vendor headers here.
    	#nordic_include_dirs,
		#cmsis_corem_include,
		inc_framework,
    ],
    c_args: [
    	# You might need processor specific definitions?
    ],
    cpp_args: [
        # You might need processor specific definitions?
    ],
    dependencies: [
    	# These dependencies will be for getting the processor to build. Common dependencies
    	# include the processor arch and common vendor driver implementations
       	# nrf_common_drivers_dep,
        # arm_dep,
    ],
    native: false
)

# This dependency is meant to be used by HW platforms built using this processor
target_processor_dep = declare_dependency(
	include_directories: [
		# These include directories should be _Safe_ from vendor includes!
		# These are only what the HW platform needs to talk to the library
		include_directories('.')
	],
	sources: [files('template_hw_platform.cpp')],
	compile_args: [
		# Include any relevant compiler args here
	],
	link_args: [
		# Put any linker arguments here, for example:
		# So the application linker script can pick up gcc_arm_common.ld
		# '-L' + arm_directory_root,
	],
	dependencies: [
		# Put build dependencies for your processor and drivers here
	],
	link_with: template_processor
)
