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
	 * @param name The name of the driver base
	 * @note DriverBase() uses a std::string_view, so name input smust remain valid
	 * @param c The type of the driver base
	 */
	TestDriverBase(const char* name, uint32_t c = embvm::DriverType::Undefined) noexcept
		: DriverBase(name, c)
	{
	}

	/// @overload TestDriverBase(const char* name, uint32_t c)
	TestDriverBase(const std::string& name, uint32_t c = embvm::DriverType::Undefined) noexcept
		: DriverBase(name, c)
	{
	}

	/// @overload TestDriverBase(const char* name, uint32_t c)
	TestDriverBase(const std::string_view& name, uint32_t c = embvm::DriverType::Undefined) noexcept
		: DriverBase(name, c)
	{
	}

	/// Default destructor
	~TestDriverBase() noexcept;

  private:
	void start_() noexcept final;
	void stop_() noexcept final;
};

} // namespace test

#endif // UNIT_TEST_DRIVER_H_
