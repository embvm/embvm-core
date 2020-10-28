#ifndef UNIT_TEST_GPIO_HPP_
#define UNIT_TEST_GPIO_HPP_

#include <catch2/catch.hpp>
#include <driver/gpio.hpp>

namespace test
{
/** Unit Test GPIO Driver
 *
 * @ingroup TestDrivers
 */
class UnitTestGPIO final : public embvm::gpio::base
{
  public:
	UnitTestGPIO() noexcept : mode_(embvm::gpio::mode::output) {}
	explicit UnitTestGPIO(embvm::gpio::mode mode) noexcept : mode_(mode) {}
	~UnitTestGPIO() noexcept = default;
	bool get() noexcept final;
	void set(bool v) noexcept final;
	void toggle() noexcept final;
	void setMode(embvm::gpio::mode mode) noexcept final;
	embvm::gpio::mode mode() noexcept final;

  protected:
	void start_() noexcept final;
	void stop_() noexcept final;

  private:
	embvm::gpio::mode mode_;
	bool value_ = false;
};

} // namespace test

#endif // UNIT_TEST_GPIO_HPP_
