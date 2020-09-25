# Embedded Framework Source Templates

This directory tree contains source file templates for creating new framework elements:

* [Drivers](driver/)
* [Hardware Platforms](hw_platform/)
* [Operating Systems](os/)
* [Platforms](platform/)
* [Processors](processor/)
* [Processor Architectures](processor_arch/)

These files provide the required build rules, interfaces, includes, and class derivations for each element. 

## Creating a New Element

1. Copy the new files to the proper location for your new element
    - Drivers go in [src/drivers](../src/drivers), under the organization scheme of `vendor`/`part`
    - Hardware platforms go in [src/hw_platforms](../src/hw_platforms)
        + The new platform may be a top-level hardware platform
        + The new platform may be part of a grouping (e.g. a new `nordic` development kit), and should be added under the appropriate grouping
    - Operating systems go in [src/os](../src/os/)
        + Create a new folder for the new OS
    - Platforms go in [src/platforms](../src/platforms)
        + The new platform may be a top-level platform
        + The new platform may be part of a grouping (e.g. a new `blinky` platform variant), and should be added under the appropriate grouping
    - Processors go in [src/processors](../src/processors), under the organization scheme of `processor_architecture`/`processor_manufacturer`/`family`/`chip`
        + Family is an optional value and is only used if appropriate for the manufacturer's product line organization scheme
    - Processor architectures go in [src/processors](../src/processors), and processor architectures are the top-level organization scheme. Create an `arch` folder for these files
        - If a top-level architecture already exists and you are making a sub-architecture (e.g. adding ARM Cortex-A5 support when Cortex-M already exists), simply add the template files to the `arch` folder.
2. Rename the files to reflect the new element rather than the template name
    - Do not rename *platform* files because the framework relies on the specific filenames
3. Open the files and perform a find + replace operation to change the template names to your element's proper name
4. Rename header guards in `.h` and `.hpp` files
5. Proceed with development

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

## Adding a New Element to the Build

add the new location to the containing directory's `meson.build` file, and proceed with development!
