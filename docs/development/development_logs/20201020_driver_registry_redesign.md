# Driver Registry Redesign Log
20201020 - Phillip Johnston

First, we want to break the dependency of the driver base class on the driver registry.

We're going to remove all of these calls:

```
embvm::DriverBase::DriverBase(const char* name, uint32_t c) noexcept : name_(name), type_(c)
{
    VirtualPlatform::registerDriver(name_, this);
}

embvm::DriverBase::DriverBase(const std::string& name, uint32_t c) noexcept : name_(name), type_(c)
{
    VirtualPlatform::registerDriver(name_, this);
}

embvm::DriverBase::DriverBase(const std::string_view& name, uint32_t c) noexcept
    : name_(name), type_(c)
{
    VirtualPlatform::registerDriver(name_, this);
}

embvm::DriverBase::~DriverBase() noexcept
{
    VirtualPlatform::unregisterDriver(name_, this);
}
```

> Does this mean we can get rid of `VirtualPlatform` and `platform.hpp`? or are there other uses?

We'll also remove the files from the build, and we'll make the constructors and destructors _default_.

```
  protected:
    /** Construct a DriverBase with a C-string name.
     *
     * @param name The name of the device driver instance
     * @param c The device type
     */
    DriverBase(const char* name, embvm::DriverType_t c = embvm::DriverType::Undefined) noexcept
        : name_(name), type_(c)
    {
    }

    /** Construct a DriverBase with a std::string name.
     *
     * @param name The name of the device driver instance.
     *  Note: DriverBase() uses a std::string_view, so the std::string must remain valid
     * @param c The device type
     */
    DriverBase(const std::string& name,
               embvm::DriverType_t c = embvm::DriverType::Undefined) noexcept
        : name_(name), type_(c)
    {
    }

    /** Construct a DriverBase with a std::string_view name.
     *
     * @param name The name of the device driver instance.
     *  Note: DriverBase() uses a std::string_view, so the std::string_view must remain valid
     * @param c The device type
     */
    DriverBase(const std::string_view& name,
               embvm::DriverType_t c = embvm::DriverType::Undefined) noexcept
        : name_(name), type_(c)
    {
    }

    /// Destructor.
    ~DriverBase() noexcept = default;
```

This refactoring also allows us to get rid of a the need to build framework core components in the platform. We can delete:

```
# Because driver_base needs access to each platform's VirtualPlatform
# Definition, we separate that file out and trust the platform will resolve it at link time
driver_platform_files = files(
    'driver_base.cpp',
)
```

From the following deps:

```
libcore_dep = declare_dependency(
    link_with: libcore,
    sources: [
        driver_platform_files,
        boot_platform_files
    ],
    dependencies: framework_include_dep,
)

libcore_native_dep = declare_dependency(
    link_with: libcore_native,
    sources: [
        driver_platform_files,
        boot_platform_files
    ],
    dependencies: framework_include_dep,
)

## TODO: hosted-native dependency - leave out boot_platform files perhaps?
## TODO: -fno-threadsafe-statics: check for compiler support first!
libcore_threadless_dep = declare_dependency(
    link_with: libcore_threadless,
    sources: [
        driver_platform_files,
        boot_platform_files,
    ],
    compile_args: ['-fno-threadsafe-statics'],
    dependencies: framework_include_dep,
)

libcore_threadless_native_dep = declare_dependency(
    link_with: libcore_threadless_native,
    sources: [
        driver_platform_files,
        boot_platform_files
    ],
    compile_args: ['-fno-threadsafe-statics'],
    dependencies: framework_include_dep,
)
```

Helpfully, now our tests are failing. We need to update them with the new model.

```
../src/core/driver/driver_test.cpp:95
...............................................................................

../src/core/driver/driver_test.cpp:100: FAILED:
  CHECK( (1 + 1) == p.driverCount() )
with expansion:
  2 == 0

-------------------------------------------------------------------------------
Find driver from registry by name
-------------------------------------------------------------------------------
../src/core/driver/driver_test.cpp:103
...............................................................................

../src/core/driver/driver_test.cpp:109: FAILED:
  CHECK( d == p.findDriver("Test base") )
with expansion:
  {?} == {?}

../src/core/driver/driver_test.cpp:110: FAILED:
  CHECK( d2 == p.findDriver("Test base2") )
with expansion:
  {?} == {?}

-------------------------------------------------------------------------------
Find one driver from registry by type
-------------------------------------------------------------------------------
../src/core/driver/driver_test.cpp:113
...............................................................................

../src/core/driver/driver_test.cpp:120: FAILED:
  CHECK( d_found )
with expansion:
  {?}

../src/core/driver/driver_test.cpp:121: FAILED:
  CHECK( d == d_found )
with expansion:
  {?} == {?}
```

First, we'll remove all instances of driver registry related tests from the driver test file.

Next, I notice that the Virtual platform tests are failing! This is because we're using the platform interfaces to find a driver.

```
Find a driver by name
-------------------------------------------------------------------------------
../src/core/platform/virtual_platform_tests.cpp:48
...............................................................................

../src/core/platform/virtual_platform_tests.cpp:55: FAILED:
  CHECK( (1 + 1) == p.driverCount() )
with expansion:
  2 == 0

../src/core/platform/virtual_platform_tests.cpp:56: FAILED:
  CHECK( TestDriver == driver )
with expansion:
  {?} == {?}

-------------------------------------------------------------------------------
Find a driver by type
-------------------------------------------------------------------------------
../src/core/platform/virtual_platform_tests.cpp:59
...............................................................................

../src/core/platform/virtual_platform_tests.cpp:66: FAILED:
  CHECK( (1 + 1) == p.driverCount() )
with expansion:
  2 == 0

../src/core/platform/virtual_platform_tests.cpp:67: FAILED:
  CHECK( TestDriver == driver )
with expansion:
  {?} == {?}

-------------------------------------------------------------------------------
Find all drivers by type
-------------------------------------------------------------------------------
../src/core/platform/virtual_platform_tests.cpp:70
...............................................................................

../src/core/platform/virtual_platform_tests.cpp:80: FAILED:
  CHECK( (1 + 4) == p.driverCount() )
with expansion:
  5 == 0

../src/core/platform/virtual_platform_tests.cpp:81: FAILED:
  CHECK( 3 == driver_list.size() )
with expansion:
  3 == 0

-------------------------------------------------------------------------------
Create unit test platform
-------------------------------------------------------------------------------
../src/core/platform/virtual_platform_tests.cpp:89
...............................................................................

../src/core/platform/virtual_platform_tests.cpp:93: FAILED:
  CHECK( 1 == p.driverCount() )
with expansion:
  1 == 0

-------------------------------------------------------------------------------
Unit test platform can find drivers
-------------------------------------------------------------------------------
../src/core/platform/virtual_platform_tests.cpp:97
...............................................................................

../src/core/platform/virtual_platform_tests.cpp:110: FAILED:
  CHECK( TestDriver2 == driver )
with expansion:
  {?} == {?}

../src/core/platform/virtual_platform_tests.cpp:111: FAILED:
  CHECK( 4 <= p.driverCount() )
with expansion:
  4 <= 0

../src/core/platform/virtual_platform_tests.cpp:112: FAILED:
  CHECK( 3 <= driver_list.size() )
with expansion:
  3 <= 0

-------------------------------------------------------------------------------
Hardware platform drivers are automatically registered
-------------------------------------------------------------------------------
../src/core/platform/virtual_platform_tests.cpp:188
...............................................................................

../src/core/platform/virtual_platform_tests.cpp:192: FAILED:
  CHECK( 0 < p.driverCount() )
with expansion:
  0 < 0

===============================================================================
test cases: 126 | 120 passed |  6 failed
assertions: 472 | 461 passed | 11 failed
```

This test suite also assumes that the platform is auto-registering drivers through the base class, so we're going to need to update those tests and get this platform passing.

First, we'll need to use the `registerDriver()` platform API to add these.

```
TEST_CASE("Find a driver by name", "[core/platform/virtual_platform]")
{
    auto& p = UnitTestPlatform::inst();
    TestDriverBase TestDriver("PlatformSPI", embvm::DriverType::SPI);
    p.registerDriver(TestDriver.name(), &TestDriver);

    auto driver = p.findDriver("PlatformSPI");

    CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 1) == p.driverCount());
    CHECK(TestDriver == driver);
}
```

This gets us closer, because we still need to register these drivers in the unit test code's hardware platform

```
../src/core/platform/virtual_platform_tests.cpp:56: FAILED:
  CHECK( (1 + 1) == p.driverCount() )
with expansion:
  2 == 1
```

However, some checks are failing for other reasons:

```
../src/core/platform/virtual_platform_tests.cpp:100: FAILED:
  CHECK( 1 == p.driverCount() )
with expansion:
  1 == 6
```

This number is so high because we are destroying drivers, but not removing them from the registry before we do so!

```
TEST_CASE("Find a driver by name", "[core/platform/virtual_platform]")
{
    auto& p = UnitTestPlatform::inst();
    TestDriverBase TestDriver("PlatformSPI", embvm::DriverType::SPI);
    p.registerDriver(TestDriver.name(), &TestDriver);

    auto driver = p.findDriver("PlatformSPI");

    CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS + 1) == p.driverCount());
    CHECK(TestDriver == driver);

    p.unregisterDriver(TestDriver.name(), &TestDriver);

    CHECK((EXPECTED_UNIT_TEST_STARTING_DRIVERS) == p.driverCount());
}
```

> Note: PJ: is there an RAII solution available to us?

This gets me back down to the expected count:

```
../src/core/platform/virtual_platform_tests.cpp:108: FAILED:
  CHECK( (1) == p.driverCount() )
with expansion:
  1 == 0
```

Now we need to handle manual registration in the unit testing hardware platform.

## Device Registry and the Hardware Platform

Now, we run into one problem here... We want to register drivers in our hardware platform via a driver registry, but we want to defer the selection of WHAT driver registry to use until we're in the platform. From the hardware-platform's perspective, we don't care if we are using static or dynamic registry creation, do we? We can't very well have our hardware platform deciding that it needs dynamic memory, when our platform decides there's no dynamic memory!

Right now, our platform base takes in a driver registry template:

```
template<typename TPlatform, class TDriverRegistry>
class VirtualPlatformBase
```

And we declare a static singleton in the virtual platform base:

```
    /** Access the platform DriverRegistry instance.
     *
     * Static declaration of the DriverRegistry for the platform.
     * We use this function to avoid the static initializatio order 'fiasco'.
     * See more: https://isocpp.org/wiki/faq/ctors#static-init-order
     */
    static TDriverRegistry& driverRegistry() noexcept
    {
        static TDriverRegistry driver_registry_;
        return driver_registry_;
    }
```

We'll delete the static singleton, then move the template declaration to the virtual hardware platform.

```
template<typename THWPlatform, class TDriverRegistry>
class VirtualHwPlatformBase
```

We'll declare a private member:

```
  private:
    const std::string_view name_;
    TDriverRegistry driver_registry_{};
```

Next, I'm going to *copy* all of the  platform-level driver registry interfaces into the hardware platform base class:

```
#pragma mark - Driver Registry Functions -

    /** Platform-level API for registering a new device driver
     *
     * Register a device driver with the platform's driver registry.
     *
     * This call forwards the information to the DriverRegistry instance.
     *
     * @param name The name (used as a key) the driver will be registered under.
     * @param driver Pointer to the embvm::DriverBase object. A pointer is used because
     *  there are any number of potential derived classes which will be tracked.
     *  To prevent slicing, a pointer to the base class is stored.
     */
    static void registerDriver(const std::string_view& name, embvm::DriverBase* driver) noexcept
    {
        driverRegistry().add(name.data(), driver);
    }

    /** Platform-level API for unregistering a new device driver
     *
     * Unregister a device driver with the platform's driver registry.
     *
     * This call forwards the information to the DriverRegistry instance.
     *
     * @param name The name of the driver to remove.
     * @param driver Pointer to the embvm::DriverBase object being removed.
     */
    static void unregisterDriver(const std::string_view& name, embvm::DriverBase* driver) noexcept
    {
        driverRegistry().remove(name.data(), driver);
    }

    /** Access a device driver in the registry by name
     *
     * Find a driver by name.
     *
     * This call forwards the information to the DriverRegistry instance.
     *
     * @returns An optional_ref to the embvm::DriverBase instance. If no instance is found,
     *  the optional reference will be invalid. The caller must cast to the appropriate type.
     */
    auto findDriver(const std::string_view& name) noexcept
    {
        return driverRegistry().find(name.data());
    }

    /** Access a device driver in the registry by type
     *
     * Find a driver by type. If multiple drivers are found for a type, the first one found will be
     *returned.
     *
     * This call forwards the information to the DriverRegistry instance.
     *
     * @returns An optional_ref to the embvm::DriverBase instance. If no instance is found,
     *  the optional reference will be invalid. The caller must cast to the appropriate type.
     */
    auto findDriver(embvm::DriverType_t type) noexcept
    {
        return driverRegistry().find(type);
    }

    /** Access a device driver in the registry by type, cast as the appropriate base class.
     *
     * If multiple drivers are found for a type, the first one found will be returned.
     * The type will be returned as the appropriate base class (instead of embvm::DriverBase).
     *
     * This call forwards the information to the DriverRegistry instance.
     *
     * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
     * @returns an type_safe::optional_ref cast to the TDriverClass type. If the driver
     *  was not found, the optional_ref will be empty.
     */
    template<class TDriverClass>
    auto findDriver() noexcept
    {
        return driverRegistry().template find<TDriverClass>();
    }

    /** Access a device driver in the registry by name, cast as the appropriate base class
     *
     * The type will be returned as the appropriate base class (instead of embvm::DriverBase).
     *
     * This call forwards the information to the DriverRegistry instance.
     *
     * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
     * @returns an type_safe::optional_ref cast to the TDriverClass type. If the driver
     *  was not found, the optional_ref will be empty.
     */
    template<class TDriverClass>
    auto findDriver(const std::string_view& name) noexcept
    {
        return driverRegistry().template find<TDriverClass>(name.data());
    }

    /** Get a list of all device drivers in the registry by type.
     *
     * The type will be returned as the appropriate base class (instead of embvm::DriverBase).
     *
     * This call forwards the information to the DriverRegistry instance.
     *
     * @param type The type of driver being requested (embvm::i2c::master, SystemClock).
     * @returns A list of embvm::DriverBase instances. If no matching types are found,
     *  an empty list will be returned. The caller must cast to the appropriate type.
     */
    auto findAllDrivers(embvm::DriverType_t type) noexcept
    {
        return driverRegistry().find_all(type);
    }

    /** Get a list of all device drivers in the registry by type, cast as the appropriate base
     *class.
     *
     * The type will be returned as the appropriate base class (instead of embvm::DriverBase).
     *
     * This call forwards the information to the DriverRegistry instance.
     *
     * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
     * @returns A list of driver instances cast as TDriverClass types. If no matching types are
     *found, an empty list will be returned.
     */
    template<class TDriverClass>
    auto findAllDrivers() noexcept
    {
        return driverRegistry().template find_all<TDriverClass>();
    }

    /** Get the count of drivers registered with the platform.
     *
     * returns Number of drivers currently registered with the platform DriverRegistry.
     */
    size_t driverCount() const noexcept
    {
        return driverRegistry().count();
    }

```

Next, I need to change from the static call to the member itself (for all functions)

```
    size_t driverCount() const noexcept
    {
        return driver_registry_.count();
    }
```

I'm also going to take this opportunity to fix the naming conventions for these interfaces.

Calls like registerDriver and unregisterDriver call registry->add and registry->remove under the hood. The difference in these names is confusing - all the other names are symmetrical with those that are used in the registry. We can change the register/unregister part in the platform/hw platform, or add/remove in the registry. I think that "register" and "unregister" make more sense, so we'll change the registry base class.

```
    static void registerDriver(const std::string_view& name, embvm::DriverBase* driver) noexcept
    {
        driver_registry_.register(name.data(), driver);
    }

    static void unregisterDriver(const std::string_view& name, embvm::DriverBase* driver) noexcept
    {
        driver_registry_.unregister(name.data(), driver);
    }
```

Ahh... but here's the problem... register is a keyword!

We can keep the asymmetry for now... I'll document it as a comment int he base class.

Ideally I'd like to unify the names in a more consistent way, like: driverFind, but in this case I just think the readability is better as p.findDriver(), p.findAllDrivers().

Now, in the virtual platform base class, we're going to forward all the driver registry APIs to the virtual hardware platform... but how...?

We'll update the platform to take in a hardware platform base class

```
template<typename TPlatform, typename THWPlatform>
```

We'll declare the private variable:

```
  protected:
    const std::string_view name_;
    THWPlatform hw_platform_{};
```

And a protected API that can be used for access:

```
  protected:
    THWPlatform& hw_platform()
    {
        return hw_platform_;
    }
```

Now we can forward the calls to the hw_platform_. We'll also make these functions *inline*:

```
    inline size_t driverCount() const noexcept
    {
        return hw_platform_.driverCount();
    }
```

Our hardware platform definitons now become a little more complex... We need some way to pipe the driver registry definition from the platform to the hw platform.

- one option is to create a hardware_platform_options.hpp in the platform and get that to work with the hardware platform. This makes testing more complicated...
- another option is to use platform_options.hpp... but that's a circular dep too.
- Another option is to have template type for the registry, but this makes for CRAZY CRTP, which is a pretty big burden on users...
```
class UnitTestHWPlatform : public embvm::VirtualHwPlatformBase<UnitTestHWPlatform<TDriverRegistry>, TDriverRegistry>
{
    using Base = embvm::VirtualHwPlatformBase<UnitTestHWPlatform<TDriverRegistry>, TDriverRegistry>;
```
- We could also provide a default with the CRTP...
```
template<class TDriverRegistry = embvm::DynamicDriverRegistry>
```

What if we removed it from the base class... but required the user to define the variable themselves?

That doesn't seem to compile...

One downside of having it as a template parameter for the hardware platform base calss is that we now have a template class - which means we can't easily implement the necessary functions in the .cpp file! So we want to find a way to specify this as an option... So maybe hw platform and platform are compiled together as one compilation unit.

We'll create a new `#include <hw_platform_options.hpp>` header which can be used to configure the hardware platform. This should live in the platform folder. In the unit tests platform folder, I'll make a new `hw_platform_options.hpp` file.

```
#ifndef UNIT_TEST_HW_PLATFORM_OPTIONS_HPP_
#define UNIT_TEST_HW_PLATFORM_OPTIONS_HPP_

#include <driver/driver_registry.hpp>
#include <mutex>

using PlatformDriverRegistry = embvm::DynamicDriverRegistry<std::mutex>;

#endif // UNIT_TEST_HW_PLATFORM_OPTIONS_HPP_
```

Then I'l use the specified option in the HW platform header:

```
class UnitTestHWPlatform : public embvm::VirtualHwPlatformBase<UnitTestHWPlatform, PlatformDriverRegistry>
{
  using Base = embvm::VirtualHwPlatformBase<UnitTestHWPlatform, PlatformDriverRegistry>;
```

Now we need to update our platform definition.

We'll remove the manual declaration:

```
  private:
    UnitTestHWPlatform hw_platform_;
```

And update the template parameters to select a hardware platform:

```
class UnitTestPlatform
    : public embvm::VirtualPlatformBase<UnitTestPlatform, UnitTestHWPlatform>,
      public embvm::PlatformDispatcher<PlatformDispatchQueue>,
      public embvm::PlatformEventManagement<PlatformEventCenter>
{
    using VirtualPlatform = embvm::VirtualPlatformBase<UnitTestPlatform, UnitTestHWPlatform>;
```

This gets our program compiling once more. But our tests are still failing! That's because we got really derailed on our mission to manually register drivers in the hardware platform.

```
    UnitTestHWPlatform() noexcept
        : Base("Unit Test HW Platform")
    {
        registerDriver("test0", &driver_);
    }
```

Now we're back to passing tests for the platform files:

```
$ /Users/phillip/src/ea/framework/embvm-core/buildresults/build/test/catch2/embvm-core_catch2_test
===============================================================================
All tests passed (482 assertions in 126 test cases)
```

We just need to take care of the driver registry tests. We'll add them to the build, and we expect there to be failures.

```
driver_test_files = files(
    'driver_test.cpp',
    'driver_registry_tests.cpp'
)
```

```
-------------------------------------------------------------------------------
TestDriverBase is automatically added to registery
-------------------------------------------------------------------------------
../src/core/driver/driver_registry_tests.cpp:29
...............................................................................

../src/core/driver/driver_registry_tests.cpp:34: FAILED:
  CHECK( (1 + 1) == p.driverCount() )
with expansion:
  2 == 1

-------------------------------------------------------------------------------
Find driver from registry by name
-------------------------------------------------------------------------------
../src/core/driver/driver_registry_tests.cpp:37
...............................................................................

../src/core/driver/driver_registry_tests.cpp:43: FAILED:
  CHECK( d == p.findDriver("Test base") )
with expansion:
  {?} == {?}

../src/core/driver/driver_registry_tests.cpp:44: FAILED:
  CHECK( d2 == p.findDriver("Test base2") )
with expansion:
  {?} == {?}

-------------------------------------------------------------------------------
Find one driver from registry by type
-------------------------------------------------------------------------------
../src/core/driver/driver_registry_tests.cpp:47
...............................................................................

../src/core/driver/driver_registry_tests.cpp:54: FAILED:
  CHECK( d_found )
with expansion:
  {?}

../src/core/driver/driver_registry_tests.cpp:55: FAILED:
  CHECK( d == d_found )
with expansion:
  {?} == {?}
```

Now to fix the tests. We'll change all of the platform references to just be driver registry references, and we'll use the proper driver registry APIs to add/remove. Note that we don't need to unregister tests when we're destructing the driver registry for each test case.

```
TEST_CASE("TestDriverBase is added to and removed from registery", "[core/driver_registry]")
{
    DynamicDriverRegistry<> driver_registry;
    TestDriverBase d("Test base");

    driver_registry.add(d.name().data(), &d);

    CHECK(1 == driver_registry.count());

    driver_registry.remove(d.name().data(), &d);

    CHECK(0 == driver_registry.count());
}
```

Now all the tests compile. But I need to do a few checks for a static driver register.

Now I'm going to pause and make sure all of the associated documentation is up-to-date before continuing on.
