#ifndef UNIT_TEST_PLATFORM_OPTIONS_HPP_
#define UNIT_TEST_PLATFORM_OPTIONS_HPP_

#include <platform/virtual_platform.hpp>
#include <platform/virtual_platform_dispatch.hpp>
#include <platform/virtual_platform_event.hpp>

using PlatformDriverRegistry = embvm::DynamicDriverRegistry<std::mutex>;
using PlatformEventCenter = embvm::DynamicEventCenter;
using PlatformDispatchQueue = embutil::DynamicDispatchQueue<>;

#endif
