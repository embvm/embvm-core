# Adding a New Hardware Platform

* `meson.build`
* `hw_platform.hpp`
* `hw_platform.cpp`

Talk about the helper script once it is created


# To get this into the build, go up one level in the directory tree and add
# a `subdir(your_hw_platform_dir)` command to the subdir group

# HW Platforms should be typically build as dependencies and added into the Platform dependency
# list. If HW Platforms include items (such as include paths) which should not be leaked into the
# main Platform, then build the HW Platform as a static library. Create a dependency which includes
# safe include paths and links against the library. The Platform will still reference the dependency.


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




See: ADR 20 - document the use of the hw platform options file to configure hardware platform paremters without creating a template class



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

## Declaring Drivers

In the hardware platform header file, we can include the `stm32_gpio.hp`p header and declare some GPIO pins:
    
```
    STM32GPIOOutput<STM32GPIOPort::C, 7> led1_pin;
    STM32GPIOOutput<STM32GPIOPort::B, 7> led2_pin;
    STM32GPIOOutput<STM32GPIOPort::B, 14> led3_pin;
```
