# Adding a New Platform

# To get this into the build, go up one level in the directory tree and add
# a `subdir(your_platform_dir)` command to the subdir group

# Platforms should be typically built as dependencies and added into the application dependency list
# If platforms include items (such as include paths) which should not be leaked into the main
# Application, then build the platform as a static library. Create a dependency which includes
# safe include paths and links against the library. The app will still reference the dependency.


// Each platform defines a standard platform.hpp header, which contains
// the implementation for the platform class. The platform header maps the specific
// platform class to the `VirtualPlatform` type (with a `using` statement). This
// ensures that our application has a common way of accessing the platform object and interface,
// rather than having to redefine the type each time we want to build the application.
// This can be viewed as an abstract interface of sorts!
#include <platform.hpp>


    // This is how we access our platform class in a portable way -
    // We use a common `VirtualPlatform` alias.
    auto& platform = VirtualPlatform::inst();



    // If a _putchar() function is defined, this will dump the contents of the buffer to the
    // associated communication channel.
    // Note that this is a platform API, not a standard logging API.
    platform.echoLogBufferToConsole();

We also need a _putchar reference. Let's add that to the platform:

```
#include <printf.h> // for _putchar definition

void _putchar(char c)
{
    // TODO:
}
```


## What is a Platform?

Platform = hw platform + memory allocation scheme + OS + framework core + framework subsystems

## Platform Bring-up Checklist

- [ ] 
- [ ] 
- [ ] 


## Template Files

* `platform_options.hpp`
* `platform_logging.hpp`
* `platform.hpp`
* `platform.cpp`
* `meson.build`

Talk about the helper script once it is created

## Build

## Selecting a Memory Allocator

Using libmemory - describe this here

## Alternatives to Find Driver

```
/** Get the tof0 Instance
*
* You don't always need find_driver() in the DriverRegistry. You can directly return a
* base-class reference from the hardware platform. This saves time from doing a lookup.
*
* It is important to return the base class, since that is how we keep our applications
* decoupled from the underlying hardware. Returning the actual ToF sensor class will
* enable coupling to the underlying harware.
*
* @returns a reference ot the tof0 instance, as the base class type.
*/
embvm::tof::sensor& tof0_inst()
{
	return tof0;
}


/// Elsewhere
	// Instead of using find_driver(), we will get the instance directly
	// from the hardware platform. We use auto& because we are receiving
	// a reference to the base class.
	auto& tof = hw_platform_.tof0_inst();
	tof.registerReadCallback([](uint16_t v) { printf("ToF Range: %u mm\n", v); });

```

## Related Documents

* [Adding a New Hardware Platform](adding_new_hw_platform.md)










See: ADR 20 - depending on your platform, you may be required to define a hw_platform_options.hpp file which is used to select configuration options such as the driver registry type

Need a template options file

## putchar implementation

You'll need to supply an implementation for `_putchar` if you're using `printf`. We recommend doing this in platform.cpp, or another platform-specific file.

If you don't need to use this functionality, but your build requires it, you can define an empty implementation:

```
// We don't print anything, so supply an empty definition
extern "C" void _putchar(char c)
{
    (void)c;
}

```


## Making it Easier on Users

If most of our platforms are going to be doing this:

```
#include <hw_platform_options.hpp>

class UnitTestHWPlatform
    : public embvm::VirtualHwPlatformBase<UnitTestHWPlatform, PlatformDriverRegistry>
```

Why don't we make it easier?

We're using C++17, so we can take advantage of `__has_include`

```
#if __has_include(<hw_platform_options.hpp>)
#include <hw_platform_options.hpp>
#endif
```

```
#if __has_include(<hw_platform_options.hpp>)
template<typename THWPlatform, class TDriverRegistry = PlatformDriverRegistry>
#else
template<typename THWPlatform, class TDriverRegistry>
#endif
```

Now I can compile the project and confirm that everything works like this since I have a configuration header defined

```
class UnitTestHWPlatform
    : public embvm::VirtualHwPlatformBase<UnitTestHWPlatform>
{
    using Base = embvm::VirtualHwPlatformBase<UnitTestHWPlatform>;
```

Note documentation from here:

```
#include <driver/driver_registry.hpp>
#include <string>
#if __has_include(<hw_platform_options.hpp>)
#include <hw_platform_options.hpp>
#endif

namespace embvm
{
/** Virtual Hardware Platform Base
 *
 * This class provides the common interfaces and behaviors that virtual hardware platforms
 * must implement. Some functionality is common to all platforms (name functions).
 *
 * Functions whose names are appended with `_` are meant to be supplied by the derived hardware
 * platform:
 *  - earlyInitHook_()
 *  - init_()
 *  - initProcessor_()
 *  - soft_reset_()
 *  - hard_reset_()
 *  - shutdown_()
 *
 * Derived classes may supply additional functions as required. The functions above are the
 * common required functions that all hardware platforms must supply.
 *
 * This class uses the @ref docs/development/patterns/crtp.md ["CRTP pattern"] rather
 * than virtual inheritance. To derive from this class, do the following:
 *  @code
 *  class FWDemoSimulatorHWPlatform : public VirtualHwPlatformBase<FWDemoSimulatorHWPlatform>
 *  { ... };
 *  @endcode
 *
 * @tparam THWPlatform the derived HW platform implementation (CRTP pattern)
 * @tparam TDriverRegistry The type of the platform's DriverRegistry. DriverRegistry type is
 *  specified to enable consumers to specify the exact DriverRegistry type/strategy in the
 *  platform layer, since the HW platform doesn't need to know anything about the memory
 *  allocation scheme.
 *  If your platform defines a hw_platform_options.hpp header, then the base class will
 *  automatically pick up your configured registry type.
 *  @code
 *  class UnitTestHWPlatform
 *  : public embvm::VirtualHwPlatformBase<UnitTestHWPlatform>
 *  @endcode
 *  If this header is missing, or you wish to override the platform default for whatever
 *  reason, you can manually specify the type.
 *  @code
 *  class UnitTestHWPlatform
 *  : public embvm::VirtualHwPlatformBase<UnitTestHWPlatform, embvm::DynamicDriverRegistry>
 *  @endcode
 *
 * @ingroup FrameworkHwPlatform
 */
#if __has_include(<hw_platform_options.hpp>)
template<typename THWPlatform, class TDriverRegistry = PlatformDriverRegistry>
#else
template<typename THWPlatform, class TDriverRegistry>
#endif
class VirtualHwPlatformBase
{
```

## Linker Scripts

Talk about linker scripts belonging in the platform

show how to manually add them to target
add link_depends for things to rebuild

talk about how to add them via meson cross files as well

https://embeddedartistry.com/blog/2020/04/27/how-to-incorporate-linker-scripts-in-meson-cross-compilation-files/

If you're using the embvm skeleton, this will already be handled by one of our modules: `build/linker/linker-script-as-property`
