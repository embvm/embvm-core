#ifndef TEMPLATE_SYSTEM_CLOCK_HPP_
#define TEMPLATE_SYSTEM_CLOCK_HPP_

#include <driver/system_clock.hpp>
#include <ratio>
#include <thread>

namespace embdrv
{
/** Document class here
 */
class TemplateSystemClock final : public embvm::clk::SystemClock
{
  public:
	TemplateSystemClock() noexcept;

	~TemplateSystemClock() noexcept final;

	embvm::clk::freq_hz_t::rep frequency() const noexcept final;

	tick_duration_t::rep ticks() const noexcept final;

	void spin(spin_duration_t::rep count) noexcept final;

  protected:
	void start_() noexcept final;
	void stop_() noexcept final;
};

} // namespace embdrv

#endif // TEMPLATE_SYSTEM_CLOCK_HPP_
