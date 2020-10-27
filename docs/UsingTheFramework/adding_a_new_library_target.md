

## Adding a New Static Library Build Target

#TODO: UPDATE THIS TO USE THE LATEST DEPENDENCY

When creating static library or executable targets, you need to specify `libc_header_include` as an `include_directories` entry and `libcxx_header_include_dep` as a `dependencies` entry:

```
nrf52840 = static_library('nrf52840',
    [
        'nrf52840.cpp',
        '../mdk/gcc_startup_nrf52840.S',
        '../mdk/system_nrf52840.c',
    ],
    include_directories: [
        nordic_include_dirs,
        cmsis_corem_include,
        framework_includes,
        libc_header_include, # <----- libc header include
    ],
    c_args: [
        '-DCONFIG_GPIO_AS_PINRESET',
        '-DNRF52840_XXAA',
    ],
    cpp_args: [
        '-DNRF52840_XXAA',
        '-DDEBUG_ASSERT_NO_STDIO',
    ],
    dependencies: [
        nrf_common_drivers_dep, 
        libcxx_header_include_dep, # <----- libcpp header dep
        arm_dep,
    ],
    native: false
)
```
