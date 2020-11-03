// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef UNIT_TEST_HW_PLATFORM_OPTIONS_HPP_
#define UNIT_TEST_HW_PLATFORM_OPTIONS_HPP_

#include <driver/driver_registry.hpp>
#include <mutex>

using PlatformDriverRegistry = embvm::DynamicDriverRegistry<std::mutex>;

#endif // UNIT_TEST_HW_PLATFORM_OPTIONS_HPP_
