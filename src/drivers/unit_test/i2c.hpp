// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef I2C_UNIT_TEST_DRIVER_HPP_
#define I2C_UNIT_TEST_DRIVER_HPP_

#include <driver/i2c.hpp>
#include <queue>
#include <vector>

namespace test
{
/** I2C Master Unit Test Driver
 *
 * This driver can be used as a "fake" for a unit test.
 *
 * You can prepare return data for a TX operation:
 *
 * @code
 * uint8_t expected[2] = {0xBE, 0xAE};
 * d.appendToRxBuffer(expected, sizeof(expected));
 * @endcode
 *
 * You can verify the data that was transmitted:
 * @code
 * CHECK(true == d.checkTxBuffer(input, sizeof(input)));
 * @endcode
 *
 * @ingroup TestDrivers
 */
class i2cTestDriver final : public embvm::i2c::master
{
  public:
	/// Create a generic I2C Master
	i2cTestDriver() noexcept {}

	/// Default destructor
	~i2cTestDriver() noexcept;

	void expects(embvm::i2c::op_t& op)
	{
		expectedOps_.push(op);
	}

	void returns(embvm::i2c::op_t& op)
	{
		returnOps_.push(op);
	}

	/// Clear the stored expected operations
	void clearExpected() noexcept
	{
		expectedOps_.resize(0);
	}

	/// Clear the stored return operations
	void clearReturns()
	{
		returnOps_.resize(0);
	}

  protected:
	void start_() noexcept final;
	void stop_() noexcept final;
	void configure_(embvm::i2c::pullups) noexcept final;
	embvm::i2c::status transfer_(const embvm::i2c::op_t& op,
								 const embvm::i2c::master::cb_t& cb) noexcept final;
	embvm::i2c::baud baudrate_(embvm::i2c::baud baud) noexcept final;
	embvm::i2c::pullups setPullups_(embvm::i2c::pullups pullups) noexcept final;

  private:
	std::vector<embvm::i2c::op_t> expectedOps_;
	std::queue<embvm::i2c::op_t> returnOps_;
};

} // namespace test

#endif // I2C_UNIT_TEST_DRIVER_HPP_
