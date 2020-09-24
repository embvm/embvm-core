#ifndef TEMPLATE_TOF_DRIVER_HPP_
#define TEMPLATE_TOF_DRIVER_HPP_

#include <driver/time_of_flight.hpp>

namespace embdrv
{
/** Document class
 */
class TemplateToFSensor final : public embvm::tof::sensor
{
  public:
	explicit TemplateToFSensor() noexcept : embvm::tof::sensor("Template ToF"), {}

	void read() noexcept final;
	void reset() noexcept final;

	[[nodiscard]] embvm::tof::distance_t
		getMaxRangeForModeDark(embvm::tof::mode m) const noexcept final;
	[[nodiscard]] embvm::tof::distance_t
		getMaxRangeForModeStrongLight(embvm::tof::mode m) const noexcept final;
	embvm::tof::mode mode(embvm::tof::mode m) noexcept final;

	void registerReadCallback(const read_cb_t& cb) noexcept final;
	void registerReadCallback(read_cb_t&& cb) noexcept final;

  private:
	void start_() noexcept final;
	void stop_() noexcept final;
};

} // namespace embdrv

#endif // TEMPLATE_TOF_DRIVER_HPP_
