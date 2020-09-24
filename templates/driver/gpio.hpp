#ifndef TEMPLATE_GPIO_HPP_
#define TEMPLATE_GPIO_HPP_

#include <driver/gpio.hpp>

namespace embdrv
{
/** Documentation
 */
template<embvm::gpio::pull TPull = embvm::gpio::pull::none>
class TemplateGPIOInputDriver final : public embvm::gpio::input<TPull>
{
  public:
	explicit TemplateGPIOInputDriver() noexcept : embvm::gpio::input<TPull>("Aardvark GPIO") {}
	explicit TemplateGPIOInputDriver(const char* name) noexcept : embvm::gpio::input<TPull>(name) {}

	/// Default Destructor
	~TemplateGPIOInputDriver() noexcept final = default;

	bool get() noexcept final;

  private:
	void start_() noexcept final;

	void stop_() noexcept final;

	embvm::gpio::pull pull_(embvm::gpio::pull p) noexcept final;

  private:
};

/** Driver Documentation
 */
class TemplateGPIOOutputDriver : public embvm::gpio::output
{
  public:
	explicit TemplateGPIOOutputDriver() noexcept : embvm::gpio::output("Aardvark GPIO") {}

	explicit TemplateGPIOOutputDriver(const char* name) noexcept : embvm::gpio::output(name) {}

	~TemplateGPIOOutputDriver() noexcept final = default;
	void set(bool v) noexcept final;

  private:
	void start_() noexcept final;
	void stop_() noexcept final;
};

template<uint8_t TPort, uint8_t TPin>
class TemplatedGPIOOutputExample final : public embvm::gpio::output
{
  public:
	explicit TemplatedGPIOOutputExample() noexcept : embvm::gpio::output("Templated GPIO Output") {}

	explicit TemplatedGPIOOutputExample(const char* name) noexcept : embvm::gpio::output(name) {}

	~TemplatedGPIOOutputExample() final = default;

	void set(bool v) noexcept final;

  private:
	void start_() noexcept final;

	void stop_() noexcept final;
};

} // namespace embdrv

#endif // TEMPLATE_GPIO_HPP_
