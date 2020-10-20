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
	i2cTestDriver() noexcept : embvm::i2c::master() {}

	/// Create a named I2C master
	/// @param name The name of this i2c instance.
	explicit i2cTestDriver(const char* name) noexcept : embvm::i2c::master(name) {}

	/// Default destructor
	~i2cTestDriver() noexcept;

	/// Verify that the driver transmitted the correct data
	/// @param data The data buffer to check.
	/// @param length The number of bytes in the input buffer to compare.
	bool checkTxBuffer(uint8_t* data, size_t length) noexcept
	{
		if(txBuffer_.size() != length)
		{
			return false;
		}

		for(size_t i = 0; i < length; i++)
		{
			if(data[i] != txBuffer_[i])
			{
				return false;
			}
		}

		return true;
	}

	/// Clear the stored TX buffer
	void clearTxBuffer() noexcept
	{
		txBuffer_.resize(0);
	}

	/// Append data to the RX buffer to be returned with the next read operation.
	/// @param data The data to append to the RX buffer
	/// @param length the number of bytes to append
	void appendToRxBuffer(uint8_t* data, size_t length) noexcept
	{
		for(size_t i = 0; i < length; i++)
		{
			rxBuffer_.push(data[i]);
		}
	}

	/// Clears the data stored in the RX buffer.
	void clearRxBuffer() noexcept
	{
		std::queue<uint8_t> empty;
		std::swap(rxBuffer_, empty);
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
	/// Storage represnting the tx buffer
	std::vector<uint8_t> txBuffer_;

	/// Storage representing the rx bufferre
	std::queue<uint8_t> rxBuffer_;
};

} // namespace test

#endif // I2C_UNIT_TEST_DRIVER_HPP_
