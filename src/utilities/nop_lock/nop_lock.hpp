// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef NOOP_LOCK_HPP_
#define NOOP_LOCK_HPP_

namespace embutil
{
/// Use this lock class to disable locking behavior at compile-time
class nop_lock
{
  public:
	void lock() noexcept;
	void unlock() noexcept;
};

} // namespace embutil

#endif
