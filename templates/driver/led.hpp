#ifndef TEMPLATE_LED_DRIVER_HPP_
#define TEMPLATE_LED_DRIVER_HPP_

#include <driver/led.hpp>

namespace embdrv
{
/** document class here
 */
class TemplateLEDDriver final : public embvm::led::base
{
  public:
	TemplateLEDDriver() noexcept : embvm::led::base("Template LED") {}

	explicit TemplateLEDDriver(const char* name) noexcept : embvm::led::base(name) {}

	~TemplateLEDDriver() noexcept final;

	void on() noexcept final;
	void off() noexcept final;
	void toggle() noexcept final;

  private:
	void start_() noexcept final;
	void stop_() noexcept final;
};
} // namespace embdrv

#endif // TEMPLATE_LED_DRIVER_HPP_
