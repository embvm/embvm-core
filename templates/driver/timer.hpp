#ifndef TEMPLATE_TIMER_HPP_
#define TEMPLATE_TIMER_HPP_

namespace embdrv
{
/** Timer class documentation
 */
class TemplateTimer final : public embvm::timer::Timer
{
  public:
	TemplateTimer() noexcept : Timer("Template Timer");

	/** Create a timer and set configuration options
	 *
	 * @param period The desired timer period.
	 * @param config The desired timer configuration (oneshot or periodic).
	 */
	explicit TemplateTimer(embvm::timer::timer_period_t period,
						   embvm::timer::config config = embvm::timer::config::oneshot) noexcept;

	/** Create a timer and set configuration options and callback
	 *
	 * @param period The desired timer period.
	 * @param cb The callback function to invoke when the timer expires.
	 * @param config The desired timer configuration (oneshot or periodic).
	 */
	explicit TemplateTimer(embvm::timer::timer_period_t period, embvm::timer::cb_t cb,
						   embvm::timer::config config = embvm::timer::config::oneshot) noexcept;

	/** Create a timer and set configuration options and callback
	 *
	 * @param period The desired timer period.
	 * @param cb The callback function to invoke when the timer expires.
	 * @param config The desired timer configuration (oneshot or periodic).
	 */
	explicit TemplateTimer(embvm::timer::timer_period_t period, embvm::timer::cb_t&& cb,
						   embvm::timer::config config = embvm::timer::config::oneshot) noexcept;

	/// Destructor, which cleans up the timer thread.
	~TemplateTimer() noexcept final;

	void registerCallback(const embvm::timer::cb_t& cb) noexcept final;
	void registerCallback(embvm::timer::cb_t&& cb) noexcept final;
	embvm::timer::timer_period_t count() const noexcept final;
	void enableInterrupts() noexcept final;
	void disableInterrupts() noexcept final;

  private:
	void start_() noexcept final;
	void stop_() noexcept final;

  private:
};

} // namespace embdrv

#endif // TEMPLATE_TIMER_HPP_
