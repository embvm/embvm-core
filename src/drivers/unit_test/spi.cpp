// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "spi.hpp"

using namespace test;

spiTestDriver::~spiTestDriver() noexcept {}

void spiTestDriver::start_() noexcept {}

void spiTestDriver::stop_() noexcept {}

void spiTestDriver::configure_() noexcept {}

void spiTestDriver::setMode_(embvm::spi::mode mode) noexcept
{
	(void)mode;
}

void spiTestDriver::setOrder_(embvm::spi::order order) noexcept
{
	(void)order;
}

uint32_t spiTestDriver::baudrate_(uint32_t baud) noexcept
{
	return baud;
}

embvm::comm::status spiTestDriver::transfer_(const embvm::spi::op_t& op,
											 const embvm::spi::master::cb_t& cb) noexcept
{
	(void)cb;

	for(size_t i = 0; i < op.length; i++)
	{
		if(op.tx_buffer)
		{
			txBuffer_.push_back(op.tx_buffer[i]);
		}
		else
		{
			txBuffer_.push_back(0);
		}

		if(op.rx_buffer)
		{
			op.rx_buffer[i] = rxBuffer_.front();
			rxBuffer_.pop();
		}
	}

	return embvm::comm::status::ok;
}
