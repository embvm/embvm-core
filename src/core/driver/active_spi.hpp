// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef ACTIVE_SPI_HPP_
#define ACTIVE_SPI_HPP_

#include "spi.hpp"
#include <active_object/active_object.hpp>

namespace embvm::spi
{
using ao_storage = std::pair<embvm::spi::op_t, const embvm::spi::master::cb_t>;

template<size_t TQueueSize = 0, typename TLock = std::mutex,
		 typename TCond = std::condition_variable>
class activeMaster final
	: public master,
	  public embutil::activeObject<embvm::spi::activeMaster<TQueueSize, TLock, TCond>,
								   embvm::spi::ao_storage, TQueueSize, TLock, TCond>
{
	using ao_base = embutil::activeObject<activeMaster, ao_storage, TQueueSize, TLock, TCond>;

  public:
	// TODO: This class should unregister the managed class from the driver registry
	// During construction, and re-add it in destruction
	explicit activeMaster(embvm::spi::master& m) noexcept : m_(m) {}
	~activeMaster() {}

	void process_(ao_storage pair) noexcept
	{
		auto& [op, cb] = pair;

		auto r = m_.transfer(op, cb);

		// We retry until safe
		// TODO: callback approach. We should register callbacks with the driver
		// That notify us when a transfer is complete... then we wake the thread and try again.
		while(r == status::busy)
		{
			r = m_.transfer(op, cb);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
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

	void configure_() noexcept final
	{
		// TODO
		// m_.configure(m_.baudrate());
	}

	void setMode_(embvm::spi::mode mode) noexcept final
	{
		m_.mode(mode);
	}

	void setOrder_(embvm::spi::order order) noexcept final
	{
		m_.order(order);
	}

	embvm::spi::status transfer_(const embvm::spi::op_t& op,
								 const embvm::spi::master::cb_t& cb) noexcept final
	{
		bool success = ao_base::enqueue({op, cb});

		return success ? embvm::spi::status::enqueued : embvm::spi::status::busy;
	}

	embvm::spi::baud_t baudrate_(embvm::spi::baud_t baud) noexcept final
	{
		return m_.baudrate(baud);
	}

  private:
	embvm::spi::master& m_;
};

} // namespace embvm::spi

#endif // ACTIVE_SPI_HPP_
