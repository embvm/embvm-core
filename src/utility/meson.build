utility_inc = [
    include_directories('.'),
]

utility_sources = [
    files('example.c')
]

clangtidy_files += [
    utility_sources,
    # Add header-only files here if you want them linted
]

# If you have a utility with a source file, add it with a subdir() command
# or add it to the `utility_sources` list above
# subdir('dir')

libutil = static_library('util',
    utility_sources,
    include_directories: utility_inc,
    install: false,
    build_by_default: false,
)

libutil_native = static_library('util_native',
    utility_sources,
    include_directories: utility_inc,
    install: false,
    build_by_default: false,
    native: true
)

utility_dep = declare_dependency(
    include_directories: utility_inc,
    link_with: libutil,
)

utility_native_dep = declare_dependency(
    include_directories: utility_inc,
    link_with: libutil_native,
)
