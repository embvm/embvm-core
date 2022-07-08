// Copyright 2022 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifdef __APPLE__
#include "_semaphore/_posix_semaphore_apple.hpp"
#else
#include "_semaphore/_posix_semaphore_default.hpp"
#endif
