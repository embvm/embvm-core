// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "rtos.hpp"

using namespace embvm;

VirtualThread::~VirtualThread() noexcept = default;

VirtualSemaphore::~VirtualSemaphore() noexcept = default;

VirtualMutex::~VirtualMutex() noexcept = default;

VirtualEventFlag::~VirtualEventFlag() noexcept = default;

VirtualConditionVariable::~VirtualConditionVariable() noexcept = default;
