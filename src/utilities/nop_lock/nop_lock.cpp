// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "nop_lock.hpp"

using namespace embutil;

void nop_lock::lock() noexcept {}

void nop_lock::unlock() noexcept {}
