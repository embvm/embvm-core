#ifndef UNIT_TEST_GPIO_HPP_
#define UNIT_TEST_GPIO_HPP_

#include <catch2/catch.hpp>
#include <driver/gpio.hpp>

// TODO: Add functions that can set the GPIO input values, so we can use this as a "fake" in
// our unit test framework

namespace test
{
/** Unit Test GPIO Input Driver
 *
 * @ingroup TestDrivers
 */
class UnitTestGPIOInput final : public embvm::gpio::input<embvm::gpio::pull::none>
{
  public:
	bool get() noexcept final;

  protected:
	embvm::gpio::pull pull_(embvm::gpio::pull p) noexcept final;
	void start_() noexcept final;
	void stop_() noexcept final;
};

/** Unit Test GPIO Input Driver, with a pullup
 *
 * @ingroup TestDrivers
 */
class UnitTestGPIOInputWithPullup final : public embvm::gpio::input<embvm::gpio::pull::pullup>
{
  public:
	bool get() noexcept final;

  protected:
	embvm::gpio::pull pull_(embvm::gpio::pull p) noexcept final;
	void start_() noexcept final;
	void stop_() noexcept final;

  private:
	embvm::gpio::pull p_ = embvm::gpio::pull::pullup;
};

} // namespace test

#endif // UNIT_TEST_GPIO_HPP_
