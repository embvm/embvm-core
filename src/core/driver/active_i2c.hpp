#ifndef ACTIVE_I2C_HPP_
#define ACTIVE_I2C_HPP_

#include "i2c.hpp"
#include <active_object/active_object.hpp>

namespace embvm::i2c
{
using ao_storage = std::pair<embvm::i2c::op_t, const embvm::i2c::master::cb_t>;

template<size_t TQueueSize = 0, typename TLock = std::mutex,
		 typename TCond = std::condition_variable>
class activeMaster final
	: public master,
	  public embutil::activeObject<embvm::i2c::activeMaster<TQueueSize, TLock, TCond>,
								   embvm::i2c::ao_storage, TQueueSize, TLock, TCond>
{
	using ao_base = embutil::activeObject<activeMaster, ao_storage, TQueueSize, TLock, TCond>;

  public:
	// TODO: This class should unregister the managed class from the driver registry
	// During construction, and re-add it in destruction
	explicit activeMaster(embvm::i2c::master& m) noexcept : m_(m) {}
	~activeMaster() = default;

	void process_(ao_storage pair) noexcept
	{
		auto& [op, cb] = pair;

		auto r = m_.transfer(op, cb);

		// We retry until safe
		// TODO: callback approach. We should register callbacks with the driver
		// That notify us when a transfer is complete... then we wake the thread and try again.
		while(r == status::busy)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			r = m_.transfer(op, cb);
		}

		// TODO: callback on failure
	}

  private:
	void start_() noexcept final
	{
		m_.start();
	}

	void stop_() noexcept final
	{
		m_.stop();
	}

	void configure_(embvm::i2c::pullups pullup) noexcept final
	{
		m_.configure(m_.baudrate(), pullup);
	}

	embvm::i2c::status transfer_(const embvm::i2c::op_t& op,
								 const embvm::i2c::master::cb_t& cb) noexcept final
	{
		bool success = ao_base::enqueue({op, cb});

		return success ? embvm::i2c::status::enqueued : embvm::i2c::status::busy;
	}

	embvm::i2c::baud baudrate_(embvm::i2c::baud baud) noexcept final
	{
		return m_.baudrate(baud);
	}

	embvm::i2c::pullups setPullups_(embvm::i2c::pullups pullups) noexcept final
	{
		return m_.pullups(pullups);
	}

  private:
	embvm::i2c::master& m_;
};

} // namespace embvm::i2c

#endif // ACTIVE_I2C_HPP_
