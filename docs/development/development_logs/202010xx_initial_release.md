# 202010xx - Initial Embedded VM Public Release

The following tasks (and more) were completed in preparation for the initial public Embedded VM release.

- [x] Update Meson required version
- [x] Revisit pinned github repositories on all of the organizations and make sure we like the top itemsn in all projects to 0.56.0 or later
- [x] Change on all framework projects: clang-format `NamespaceIndentation: Inner` -> `NamespaceIndentation: None`
- [x] Update clang-format rules: find: /Users/phillip/src/ea/framework/embvm-demo-platforms/test: No such file or directory
- [x] Fix in all makefiles
    - [x] in the skeleton
    - [x] in the course:
    ```
        -       INTERNAL_OPTIONS += '-Ddebug=true -Doptimization=g'
    +       INTERNAL_OPTIONS += -Ddebug=true -Doptimization=g
    ```
- [x] Can I undo this change?
    ```
    +// Reserve space for linker symbols used by this file.
    +.word __etext
    +.word __data_start__
    +.word __bss_start__
    ```
- [x] Audit all projects to make sure we're setting cpp flags on both items:
    ```
    if enable_threading == false
        # This will stop the compiler from emiting __cxa_guard_acquire and __cxa_guard_release
        desired_cpp_compile_flags += ['-fno-threadsafe-statics']
    endif

    ALSO: need to audit all projects to make sure it's duplicated appropriately! I don't think it is in embvm-core


    if get_option('enable-threading') == false
        # This will stop the compiler from emiting __cxa_guard_acquire and __cxa_guard_release
        desired_cpp_compile_flags += ['-fno-threadsafe-statics']
        desired_native_cpp_compile_flags += ['-fno-threadsafe-statics']
    endif
    ```
- [x]Include a local copy of the CMSIS archive so we dont' have to clone?
- - [x] STM32F4 Bring-up
    * Monolithic: https://github.com/STMicroelectronics/STM32CubeL4
    * cmsis device component (startup files and such): https://github.com/STMicroelectronics/cmsis_device_l4
    * hal driver component: https://github.com/STMicroelectronics/stm32l4xx_hal_driver
    * Notes: 
        * Do we have a stm32-peripheral-drivers repository which has a common set of drivers for STM32 HAL support? I.e., we have one set that works with most systems. Then the stm32f4 series would link against the correct implementations of the drivers.
            * Need to check if they're different across families. If we have a set per family, it should just stay in the processor repo.
- [x] STM32F4 NUCLEO Board Bring-up
    * Look for BSP related code here: https://github.com/STMicroelectronics/STM32CubeL4
- [x] STM32F4 Platform files
- [x] STM32F4 Blinky Application
- [x] According to [[3. Resources/Framework-Related/Virtuality]], we should make our virtual base classes protected and non-virtual to prevent deleting destructors [base class destructors made non-virtual and protected]
- [x] Update layered view images + documentation in the framework repo - they changed in visual paradigm
- [x] Do platforms and hardware platforms really need a name?
    - NO!
- [x] Audit project wrap files: use https: insetad of git
- [x] switch to gsl-lite instead of gsl
- [x] Set the appropriate GSL definition in the build
    - THIS IS PROPERLY HANDLED THROUGH GSL_LITE
    - You can control the behaviour using a proper #define.
    - See gsl_assert file: github.com/Microsoft/GSL/include/gsl/gsl_assert.
    ```
    // 1. GSL_TERMINATE_ON_CONTRACT_VIOLATION: 
    //       std::terminate will be called (default)
    // 2. GSL_THROW_ON_CONTRACT_VIOLATION: 
    //       a gsl::fail_fast exception will be thrown
    // 3. GSL_UNENFORCED_ON_CONTRACT_VIOLATION: 
    //       nothing happens
    ```
- [x] wrap files - clone to depth 1
- [x] Audit + update subprojects for all repositories to make sure forwarding wraps are correctly defined
- [x] Remove these lines from emvbm project shims:
    ```
    # Use Embedded Artistry libc/libcpp by default for cross-compilation
    INTERNAL_OPTIONS += --cross-file=build/cross/libc.txt --cross-file=build/cross/libcpp.txt
    ```
    - Since the framework is always using libc and libcpp internally (for now)
    - Remove the libc.wrap and libcpp.wrap files too.
- [x] Create an embvm-project-skeleton repository that can be used as a quickstart, with the dependencies, shims, and gitignores properly adjusted. Should templates be included here, or in embvm-core?
- [x] GPIO class improvement - define PORTA .. PORTI typedef which is mapped to an integer, which we can safely use in the GPIO classes
    - (Check STM32 example - refactor into embvm-core)
- [x] Audit asserts - remove strings, make them comments: `     assert(0 && "Shutdown not implemented for this platform\n");`
- [x] CRTP base classes (platform, proc, hw) and active object - make wrapper calls inline!
- [x] Framework: Should the boot sequencer call the HW early init hook, rather than expecting us to forward things to the hw platform?
    - Verdict; No - because we don't know about the hw platform at the boot code level. And we want the user to have control over the process.
- [x] Audit options: make ea-framework the default, not pthread
    ```
    option('libcxx-thread-library', type: 'combo', choices: ['', 'none', 'pthread', 'ea-framework'], value: 'ea-framework', yield: true)
    ```
- [x] Update Makefile shim to make Meson binary configurable
    - [x] Propagate this change to course
    - [x] Propagate change to the skeleton
- [x] Document supporting libraries that are used
    - libc
    - libcpp
    - libmemory
    - compiler-rt (builtins)
    - gsl-lite
    - ETL
- [x] Document how to manually run the test target (is there a make shim command?)
    - Actual path: buildresults/build/test/catch2/embvm-core_catch2_test
- [x] Documentation note Platform = hw platform + memory allocation scheme + OS + framework core + framework subsystems
- [x] Create framework conventions document, and document these conventions (and any others that come to mind). Audit hte framework core to make sure we're following these guidelines. Also, add any notes about general style, formatting rules, naming conventions, namespacing, file organization, to this document.
    - [x] You can make it a convention in your code base that pointers represent non-owning `nullable` references. For instances, if `deathstar` was a global value, we might say:
    - [x] If you want to have an `optional` reference to a global variable, pointers are a superior choice: an `optional` will make a copy of the global variable, while pointers will not. 
    - [x] Incorporate: Design Patterns (Done - now dead link)
    - [x] `optional` is a good solution for resources owned by the current scope. 
    - [x] Dead link now - 20201019 - Add Tuple-Array Tools to embvm-core
- [x] Find a way to disable -nostdinc++ for C files using dependencies
    - The current solution is just to disable the warning in the relevant projects.
- [x] Should "logging_subsystem_dep" be part of framework core dep, or separate?
    - separate... because it's a subsystem!
- [x] Completed - 20201020 - Driver Registery Redesign Project
- [x] Document this process as a practical architecture article:  20201020 - Driver Registry Redesign Log
    - [x] Associated website/behind the scenes article: keeping design and development notes/logs in your repository. Show how I summarized my changes in an ADR and updates to my component specifications, while also copying the notes I made along the way into the development log folder. If you wanted a detailed look at what was done and why, you can see this log.
        - This is "free" documentation - I was already making these notes myself (because I'm scattered and it's a big effort, I needed to track what I've already done and what was left to do - and it's not going to go out of date, since it's a log of one event. But it might serve useful to others in the future to see WHY a change was made. Even for future me, which is who most of my own documentation is for.
- [x] Aardvark repo README 
    - link to user manual
        * https://www.totalphase.com/support/articles/200468316-Aardvark-I2C-SPI-Host-Adapter-User-Manual
    * Include pinout:
        * https://www.totalphase.com/support/articles/200468316-Aardvark-I2C-SPI-Host-Adapter-User-Manual#s2.1
        * 2.1.3 Order of Leads
        * 1 SCL
        * 2 GND
        * 3 SDA
        * 4 NC/+5V
        * 5 MISO
        * 6 NC/+5V
        * 7 SCLK
        * 8 MOSI
        * 9 SS
        * 10 GND
    * Also extract image of pinout 🙂
- [x] Figure out how to handle the forwarding wrap files - do we commit files that point to embvm-core dependencies? Do we .gitignore them?
    - yes - we gitignore them
- [x] Framework: Have the boot sequencer call the HW early init hook, rather than expecting the platform to forward things to the hw platform? Or is this done because we dont' have a good way to access it?
    ```
    static void earlyInitHook() noexcept
    {
        THWPlatform::earlyInitHook_();
    }
    ```
    - Can't do because the boot sequencer doesn't have access to the HW platform, and we want to give users the flexibility needed for their customization
- [x] GPIO class overhaul
- [x] Need to break this dependency chain: why does boot really need aardvark.h?
    * ../subprojects/embvm-core/src/core/boot/boot.cpp
    * In file included from ../subprojects/embvm-core/src/core/boot/boot.cpp:2:
    * In file included from ../subprojects/embvm-demo-platforms/src/platforms/blinky/blinky_sim/platform.hpp:5:
    * In file included from ../subprojects/embvm-demo-platforms/src/hw_platforms/simulator/blinky/blinky_simulator_hw_platform.hpp:4:
    * ../subprojects/aardvark/src/aardvark/base.hpp:4:10: fatal error: 'aardvark.h' file not found
    * `#include <aardvark.h>`
    * Broken by fixing the underlying class
- [x] Enable CI for new organizations
    - [x] CI Process: perform "meson subprojects update" for CI & Nightly builds
        - [ ] Update courses with a note for this
- [x] Update READMEs and documentation for projects
- [x] Update the embvm-core license to GPL before initial release
    - [x] Research Miro Samek's dual license - or ask him about it directly
    - [x] Add comment to all of the course files: //SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License
    - https://www.gnu.org/licenses/gpl-3.0.en.html
- [x] **Text to reuse in READMEs:**
    - Note that this project is dual-licensed. The open-source version of this project is released under GPLv3. [A commercial license](https://embeddedartistry.com/product/embedded-virtual-machine-commercial-license), which provides holders with the ability to create and distribute software without open-source obligations, can be purchased in the [Embedded Artistry store](https://embeddedartistry.com/store).
    - For licensing section
        Copyright © 2020 Embedded Artistry LLC
        
        Note that this project is dual-licensed. The open-source version of this project is released under GPLv3. [Commercial licenses](https://embeddedartistry.com/product/embedded-virtual-machine-commercial-license), which provide holders with the ability to create and distribute software without open-source obligations, can be purchased in the [Embedded Artistry store](https://embeddedartistry.com/store). For other open-source licenses, please see the [Software Inventory](docs/software_inventory.xlsx).
        
        See the [LICENSE](LICENSE) file for further licensing details.
- [x] Document STM32 processor bring-up process as a full example
    - See: STM32L4 Processor Bringup (note deleted - completed)
    - See: [[[Practical Arch Article] Creating Abstractions for an SDK]]
- [x] Implement I2C driver for STM32
- [x] Implement timer driver for STM32
- [x] embvm-demo works with STM32
- [x] Go through all repos and commit edits - rebuild all projects as-is, make a copy of subprojects for backup, delete subprojects, and re-build from clean repo. This will make sure everything is properly committed.
