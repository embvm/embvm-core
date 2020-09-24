#include "driver.hpp"
#include "driver_registry.hpp"
#include <platform.hpp> // For the definition of VirtualPlatform & PlatformDriverRegistry

using namespace embvm;

/*
 * VirtualPlatform is defined in each platform's platform.hpp file.
 * This file is not actually compiled as part of libcore, but instead
 * makes its way into the build as part of the platform's build.
 */

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
