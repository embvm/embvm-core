#ifndef TEMPLATE_SPI_DRIVER_HPP_
#define TEMPLATE_SPI_DRIVER_HPP_

#include <driver/spi.hpp>

namespace embdrv
{
/** Documentation
 */
class TemplateSPIMaster final : public embvm::spi::master
{
  public:
	explicit TemplateSPIMaster() noexcept : embvm::spi::master("Template SPI (Master)") {}

	explicit TemplateSPIMaster(const char* name) noexcept : embvm::spi::master(name) {}

	/// Default destructor.
	~TemplateSPIMaster() noexcept final;

  private:
	void start_() noexcept final;
	void stop_() noexcept final;
	void configure_() noexcept final;
	uint32_t baudrate_(uint32_t baud) noexcept final;
	embvm::comm::status transfer_(const embvm::spi::op_t& op,
								  const embvm::spi::master::cb_t& cb) noexcept final;
	void setMode_(embvm::spi::mode mode) noexcept final;
	void setOrder_(embvm::spi::order order) noexcept final;

  private:
};

} // namespace embdrv

#endif // TEMPLATE_SPI_DRIVER_HPP_
