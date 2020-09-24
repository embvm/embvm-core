#ifndef SIMULATOR_TIMER_HPP_
#define SIMULATOR_TIMER_HPP_

#include <atomic>
#include <condition_variable>
#include <driver/timer.hpp>
#include <thread>

// clang-format off
#include <driver/hal_driver.hpp>// This has to be last b/c of an OS X include pollution
// clang-format on

#if 0 // TODO: play around with templates
There's no specific timer in the standard library, but it's easy enough to implement one:

#include <thread>

template <typename Duration, typename Function>
void timer(Duration const & d, Function const & f)
{
    std::thread([d,f](){
        std::this_thread::sleep_for(d);
        f();
    }).detach();
}
#endif

namespace embdrv
{
/** Simulator timer driver
 *
 * This driver uses a thread object to simulate a timer with a callback.
 *
 * @ingroup SimulatorDrivers
 */
class SimulatorTimer final : public embvm::timer::Timer, public embvm::HALDriverBase
{
  public:
	/// Create a simulator timer with default options.
	SimulatorTimer() noexcept : Timer("Simulator Timer"), cb_(nullptr)
	{
		period_ = embvm::timer::timer_period_t(0);
		config_ = embvm::timer::config::oneshot;
	}

	/** Create a timer and set configuration options
	 *
	 * @param period The desired timer period.
	 * @param config The desired timer configuration (oneshot or periodic).
	 */
	explicit SimulatorTimer(embvm::timer::timer_period_t period,
							embvm::timer::config config = embvm::timer::config::oneshot) noexcept
		: Timer("Simulator Timer"), cb_(nullptr)
	{
		period_ = period;
		config_ = config;
	}

	/** Create a timer and set configuration options and callback
	 *
	 * @param period The desired timer period.
	 * @param cb The callback function to invoke when the timer expires.
	 * @param config The desired timer configuration (oneshot or periodic).
	 */
	explicit SimulatorTimer(embvm::timer::timer_period_t period, embvm::timer::cb_t cb,
							embvm::timer::config config = embvm::timer::config::oneshot) noexcept
		: Timer("Simulator Timer"), cb_(std::move(cb))
	{
		period_ = period;
		config_ = config;
	}

	/** Create a timer and set configuration options and callback
	 *
	 * @param period The desired timer period.
	 * @param cb The callback function to invoke when the timer expires.
	 * @param config The desired timer configuration (oneshot or periodic).
	 */
	explicit SimulatorTimer(embvm::timer::timer_period_t period, embvm::timer::cb_t&& cb,
							embvm::timer::config config = embvm::timer::config::oneshot) noexcept
		: Timer("Simulator Timer"), cb_(std::move(cb))
	{
		period_ = period;
		config_ = config;
	}

	/// Destructor, which cleans up the timer thread.
	~SimulatorTimer() noexcept final;

	void registerCallback(const embvm::timer::cb_t& cb) noexcept final
	{
		cb_ = cb;
	}

	void registerCallback(embvm::timer::cb_t&& cb) noexcept final
	{
		cb_ = std::move(cb);
	}

	embvm::timer::timer_period_t count() const noexcept final
	{
		return std::chrono::duration_cast<embvm::timer::timer_period_t>(
				   std::chrono::steady_clock::now().time_since_epoch()) -
			   time_base_;
	}

	void enableInterrupts() noexcept final;
	void disableInterrupts() noexcept final;

  private:
	void start_() noexcept final;
	void stop_() noexcept final;

  private:
	/// Thread which simulates a timer.
	/// We sleep the thread for the requested period using a std::condition_variable::wait_for().
	/// When the timer expires, we call the callback.
	/// We can also be notified early to cancel the timer.
	void timer_thread() noexcept;

	std::thread timer_thread_{}; // Creates an "empty" thread which doesn't execute
	embvm::timer::cb_t cb_{nullptr};
	std::atomic<bool> quit_ = false;
	std::mutex mutex_{};
	std::condition_variable cv_{};
	embvm::timer::timer_period_t time_base_{0};
};

} // namespace embdrv

#endif // SIMULATOR_TIMER_HPP_
