#ifndef TEMPLATE_I2C_DRIVER_HPP_
#define TEMPLATE_I2C_DRIVER_HPP_

#include <driver/i2c.hpp>

namespace embdrv
{
/** Document class
 */
class TemplateI2CMaster final : public embvm::i2c::master
{
  public:
	explicit TemplateI2CMaster() noexcept : embvm::i2c::master("Template I2C (Master)") {}

	explicit TemplateI2CMaster(const char* name) noexcept : embvm::i2c::master(name) {}

	/// Default destructor
	~TemplateI2CMaster() noexcept final;

  private:
	void configure_(embvm::i2c::pullups pullup) noexcept final;
	embvm::i2c::status transfer_(const embvm::i2c::op_t& op,
								 const embvm::i2c::master::cb_t& cb) noexcept final;
	embvm::i2c::baud baudrate_(embvm::i2c::baud baud) noexcept final;
	embvm::i2c::pullups setPullups_(embvm::i2c::pullups pullups) noexcept final;
	void start_() noexcept final;
	void stop_() noexcept final;
};

} // namespace embdrv

#endif // TEMPLATE_I2C_DRIVER_HPP_
