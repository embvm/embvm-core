
libexample = static_library('example',
    'example.c',
    build_by_default: false,
)

libexample_native = static_library('example_native',
    'example.c',
    build_by_default: false,
    native: true
)

libexample_dep = declare_dependency(
	include_directories: lib_inc,
	link_with: libexample,
)

libexample_native_dep = declare_dependency(
	include_directories: lib_inc,
	link_with: libexample_native,
)
