project('embvm-core',
	['cpp', 'c'],
	default_options : [
		'warning_level=3',
		'werror=false',
		# This project defaults to a release build
		'debug=false',
		'optimization=2',
		'default_library=static',
		'b_staticpic=true',
		'b_lto=false',
		# `build.*` options affect `native: true targets`
		# plain options affect `native: false` targets.
		'c_std=c11', 'build.c_std=c11',
		'cpp_std=c++17', 'build.cpp_std=c++17',
	],
	license: 'Proprietary',
	meson_version: '>=0.58.0',
	version: '0.9'
)

if meson.is_cross_build()
	target_compiler = meson.get_compiler('cpp', native: false)
	if target_compiler.get_id() == 'gcc' and target_compiler.version().version_compare('<9.0') and target_architecture == 'arm'
		error('This project requires gcc-arm-none-eabi v9.0 or later. See README.md for more information.')
	endif
endif

################################
# Project Options and Settings #
################################

# Pick up our common compiler variables + desired_*_flags variables
subdir('meson/compiler')
subdir('meson/compiler/c')
subdir('meson/compiler/cpp')

enable_threading = get_option('enable-threading')

if get_option('debug')
	add_project_arguments('-DDEBUG',
		language: ['c', 'cpp'])
	add_project_arguments('-DDEBUG',
		language: ['c', 'cpp'], native: true)
endif

if get_option('disable-builtins')
	desired_common_compile_flags += '-fno-builtin'
endif

if get_option('disable-stack-protection')
	desired_common_compile_flags += '-fno-stack-protector'
endif

if get_option('disable-rtti')
	message('Building embvm with RTTI disabled.')
	desired_cpp_compile_flags += '-fno-rtti'
	desired_native_cpp_compile_flags += '-fno-rtti'
else
	message('Building embvm with RTTI enabled.')
endif

if get_option('disable-exceptions')
	message('Building embvm with exceptions disabled.')
	desired_cpp_compile_flags += ['-fno-exceptions', '-fno-unwind-tables']
	desired_native_cpp_compile_flags += ['-fno-exceptions', '-fno-unwind-tables']
else
	message('Building embvm with exceptions enabled.')
endif

if get_option('enable-pedantic')
	desired_common_compile_flags += '-pedantic'
else
	desired_common_compile_flags += '-Wno-pedantic'
endif

if get_option('enable-pedantic-error')
	desired_common_compile_flags += '-pedantic-error'
endif

if enable_threading == false
	# This will stop the compiler from emiting __cxa_guard_acquire and __cxa_guard_release
	desired_cpp_compile_flags += ['-fno-threadsafe-statics']
	desired_native_cpp_compile_flags += ['-fno-threadsafe-statics']
endif

# Silence the warning when -nostdinc and -nostdinc++ are specified together
desired_common_compile_flags += ['-Wno-unused-command-line-argument']

compile_settings_list = [
	{'lang': 'c', 'compiler': host_c_compiler, 'flags': desired_c_compile_flags, 'isnative': false},
	{'lang': 'c', 'compiler': native_c_compiler, 'flags': desired_native_c_compile_flags, 'isnative': true},
	{'lang': 'cpp', 'compiler': host_cpp_compiler, 'flags': desired_cpp_compile_flags, 'isnative': false},
	{'lang': 'cpp', 'compiler': native_cpp_compiler, 'flags': desired_native_cpp_compile_flags, 'isnative': true},
]

link_settings_list = [
	{'lang': 'c', 'compiler': host_c_compiler, 'flags': [], 'isnative': false},
	{'lang': 'c', 'compiler': native_c_compiler, 'flags': [], 'isnative': true},
	{'lang': 'cpp', 'compiler': host_cpp_compiler, 'flags': [], 'isnative': false},
	{'lang': 'cpp', 'compiler': native_cpp_compiler, 'flags': [], 'isnative': true},
]

# Process the compilation flags
subdir('meson/compiler/check-and-apply-flags')

#################
# Build Modules #
#################

# Include reusable build modules here via subdir() calls
subdir('meson/linker/linker-script-as-property')
subdir('meson/linker/linker-map')
subdir('meson/test/cmocka')
subdir('meson/objcopy')

###############
# Subprojects #
###############

etl = subproject('etl')
etl_dep = etl.get_variable('etl_dep')

gsl_lite = subproject('gsl-lite')
gsl_lite_dep = gsl_lite.get_variable('gsl_lite_dep')

libmemory = subproject('libmemory')
libmemory_dep = libmemory.get_variable('libmemory_dep')
libmemory_native_dep = libmemory.get_variable('libmemory_native_dep')
libmemory_hosted_dep = libmemory.get_variable('libmemory_hosted_dep')
libmemory_hosted_native_dep = libmemory.get_variable('libmemory_hosted_native_dep')
libmemory_freelist_dep = libmemory.get_variable('libmemory_freelist_dep')
libmemory_threadx_dep = libmemory.get_variable('libmemory_threadx_dep')
libmemory_freertos_dep = libmemory.get_variable('libmemory_freertos_dep')
libmemory_header_include =  libmemory.get_variable('libmemory_system_includes')
libmemory_framework_rtos_dep = libmemory.get_variable('libmemory_framework_rtos_dep')

libc = subproject('libc')
libc_dep = libc.get_variable('libc_dep')
printf_dep = libc.get_variable('printf_dep')
libc_native_dep = libc.get_variable('libc_native_dep')
libc_hosted_dep = libc.get_variable('libc_hosted_dep')
libc_hosted_native_dep = libc.get_variable('libc_hosted_native_dep')
libc_header_include = libc.get_variable('libc_header_include')
libc_native_header_include = libc.get_variable('libc_native_header_include')

libcpp = subproject('libcpp')
libcxx_full_dep = libcpp.get_variable('libcxx_full_dep')
libcxx_full_native_dep = libcpp.get_variable('libcxx_full_native_dep')
libcxx_header_include_dep = libcpp.get_variable('libcxx_header_include_dep')
libcxx_native_header_include_dep = libcpp.get_variable('libcxx_native_header_include_dep')
libcxx_extensions_include_dir = libcpp.get_variable('libcxx_extensions_include_dir')
libcxxabi_dep = libcpp.get_variable('libcxxabi_dep')
libcxxabi_native_dep = libcpp.get_variable('libcxxabi_native_dep')

compiler_rt = subproject('compiler-rt')
compiler_rt_builtins_dep = compiler_rt.get_variable('compiler_rt_builtins_dep')
compiler_rt_builtins_native_dep = compiler_rt.get_variable('compiler_rt_builtins_native_dep')

#######################################
# Construct Standard Lib Dependencies #
#######################################
libc_hosted_native_partial_lib_dep = libc_hosted_native_dep.partial_dependency(links: true)
libc_hosted_native_partial_rest_dep = libc_hosted_native_dep.partial_dependency(compile_args : true, link_args : true, includes : true, sources : true)
libcxx_full_native_partial_lib_dep = libcxx_full_native_dep.partial_dependency(links: true)
libcxx_full_native_partial_rest_dep = libcxx_full_native_dep.partial_dependency(compile_args : true, link_args : true, includes : true, sources : true)
libcxxabi_native_partial_lib_dep = libcxxabi_native_dep.partial_dependency(links: true)
libcxxabi_native_partial_rest_dep = libcxxabi_native_dep.partial_dependency(compile_args : true, link_args : true, includes : true, sources : true)

if host_machine.cpu_family() == 'x86_64'
	chosen_libc_host_dep = libc_hosted_dep
else
	chosen_libc_host_dep = libc_dep
endif

libc_partial_lib_dep = chosen_libc_host_dep.partial_dependency(links: true)
libc_partial_rest_dep = chosen_libc_host_dep.partial_dependency(compile_args : true, link_args : true, includes : true, sources : true)
libcxx_full_partial_lib_dep = libcxx_full_dep.partial_dependency(links: true)
libcxx_full_partial_rest_dep = libcxx_full_dep.partial_dependency(compile_args : true, link_args : true, includes : true, sources : true)
libcxxabi_partial_lib_dep = libcxxabi_dep.partial_dependency(links: true)
libcxxabi_partial_rest_dep = libcxxabi_dep.partial_dependency(compile_args : true, link_args : true, includes : true, sources : true)

stdlib_native_dep = declare_dependency(
	dependencies: [
		libc_hosted_native_partial_rest_dep,
		libcxx_full_native_partial_rest_dep,
		libcxx_full_native_partial_lib_dep,
		libcxxabi_native_dep,
		libmemory_hosted_native_dep,
		libc_hosted_native_partial_lib_dep,
		compiler_rt_builtins_native_dep
	],
)

stdlib_host_dep = declare_dependency(
	dependencies: [
		libc_partial_rest_dep,
		libcxx_full_partial_rest_dep,
		libcxx_full_partial_lib_dep,
		libcxxabi_dep,
		libmemory_hosted_dep,
		libc_partial_lib_dep,
		compiler_rt_builtins_dep,
	],
)

# Configure the project to include `libc` headers by default

#######################
# Process Source Tree #
#######################

# Add files to this variable if you want them analyzed by clang-tidy
clangtidy_files = []
# Add dependencies to this array for testing with Catch2
catch2_tests_dep = []
# Add files to exclude from clang-format here
clangformat_excludes = []

subdir('src')
subdir('test')

# Defined after src and test so catch2_dep is fully populated
# when creating the built-in targets
subdir('meson/test/catch2')

###################
# Tooling Modules #
###################

subdir('meson/analysis/clang-tidy')
subdir('meson/analysis/complexity')
subdir('meson/analysis/cppcheck')
subdir('meson/analysis/sloccount')
subdir('meson/analysis/vale')
subdir('meson/format')
subdir('meson/docs/doxygen')

#############
# Packaging #
#############

# These macros allow you to simplify the declaration of includes for common
# include paths. Examples:
#	build_root_include.format('doc'),
#	app_include.format('APP')
#build_root_include = meson.project_build_root() + ':@0@'
#app_include = meson.project_build_root() / 'src/app:@0@'
#lib_include = meson.project_build_root() / 'src/lib:@0@'

#host_pkg_files = [
#	build_root_include.format('docs'),
#	app_include.format('APP')
#]

#native_pkg_files = [
#	build_root_include.format('docs'),
#	app_include.format('APP_native')
#]

# Invoke the package module
#subdir('meson/package')
