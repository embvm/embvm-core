# Template Platform Build Defintion

template_platform_inc = include_directories('.')

skleton_platform_dep = declare_dependency(
    sources: files('platform.cpp'),
    include_directories: template_platform_inc,
    dependencies: [
        skeleton_hw_platform_dep,
        # Add any OS-related dependencies here
        framework_dep
        # Alternatives:
	        # framework_threadless_dep
    	    # framework_native_dep
    	    # framework_threadless_native_dep
    ],
    link_args: [
    	# Linker script placeholder:
        #'-L' + meson.current_source_dir(),
        #'-Tskeleton_linker_script.ld',
    ],
)

catch2_tests_dep += declare_dependency(
    sources: files('platform_tests.cpp'),
    include_directories: include_directories('.'),
)
