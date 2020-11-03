// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef UNIT_TEST_DRIVER_H_
#define UNIT_TEST_DRIVER_H_

#include <driver/driver.hpp>

namespace test
{
/** Test driver that enables testing DriverBase
 *
 * @ingroup TestDrivers
 */
class TestDriverBase final : public embvm::DriverBase
{
  public:
	/** Create a driver base
	 *
	 * @param c The type of the driver base
	 */
	TestDriverBase(uint32_t c = embvm::DriverType::Undefined) noexcept : DriverBase(c) {}

	/// Default destructor
	~TestDriverBase() noexcept;

  private:
	void start_() noexcept final;
	void stop_() noexcept final;
};

} // namespace test

#endif // UNIT_TEST_DRIVER_H_
