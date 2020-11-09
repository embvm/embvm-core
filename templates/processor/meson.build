# Template Processor build rules

# This should be added to the containing build file's test file list
template_processor_test_files = files('template_processor_tests.cpp')

template_processor = static_library('template_processor',
    [
    	'template_processor.cpp',
    	# Include other files you might need:
        # Startup files, system initialization code
    ],
    include_directories: [
    	# These include directories are for building the library. You can safely
    	# include vendor headers here.
        include_directories('internal'),
        #nordic_include_dirs,
        #cmsis_corem_include,
    ],
    c_args: [
    	# You might need processor-specific definitions?
    ],
    cpp_args: [
        # You might need processor-specific definitions?
    ],
    dependencies: [
        framework_include_dep,
        framework_host_include_dep,
    	# These dependencies will be for getting the processor to build. Common dependencies
    	# include the processor arch and common vendor driver implementations
       	# nrf_common_drivers_dep,
        # arm_dep,
    ],
    native: false,
    build_by_default: meson.is_subproject() == false
)

# This dependency is meant to be used by HW platforms built using this processor
template_processor_dep = declare_dependency(
	include_directories: [
		# These include directories should be _Safe_ from vendor includes!
		# These includes are only what the HW platform needs to talk to the processor library.
        # Note, for example, that `./internal` is not included in the dependency
        # even though it was included in the library build.
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

catch2_tests_dep += declare_dependency(
    sources: files('template_processor_tests.cpp'),
    dependencies: template_processor_dep
)

