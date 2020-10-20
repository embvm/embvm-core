#ifndef UNIT_TEST_HW_PLATFORM_OPTIONS_HPP_
#define UNIT_TEST_HW_PLATFORM_OPTIONS_HPP_

#include <driver/driver_registry.hpp>
#include <mutex>

using PlatformDriverRegistry = embvm::DynamicDriverRegistry<std::mutex>;

#endif // UNIT_TEST_HW_PLATFORM_OPTIONS_HPP_
