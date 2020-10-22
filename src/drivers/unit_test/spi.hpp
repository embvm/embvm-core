#ifndef SPI_UNIT_TEST_DRIVER_HPP_
#define SPI_UNIT_TEST_DRIVER_HPP_

#include <driver/spi.hpp>
#include <queue>
#include <vector>

namespace test
{
/** SPI Master Unit Test Driver
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
class spiTestDriver final : public embvm::spi::master
{
  public:
	/// Create a generic spi driver with a dispatcher
	/// @param dispatcher The function to use as a dispatcher for callbacks.
	explicit spiTestDriver(const embvm::spi::master::DispatcherFunc& dispatcher = nullptr) noexcept
		: embvm::spi::master(dispatcher)
	{
	}

	/// Default destructor
	~spiTestDriver() noexcept;

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

  private:
	void start_() noexcept final;
	void stop_() noexcept final;
	void setMode_(embvm::spi::mode mode) noexcept final;
	void setOrder_(embvm::spi::order order) noexcept final;
	embvm::comm::status transfer_(const embvm::spi::op_t& op,
								  const embvm::spi::master::cb_t& cb) noexcept final;
	uint32_t baudrate_(uint32_t baud) noexcept final;
	void configure_() noexcept final;

	std::vector<uint8_t> txBuffer_;
	std::queue<uint8_t> rxBuffer_;
};

} // namespace test

#endif // SPI_UNIT_TEST_DRIVER_HPP_
