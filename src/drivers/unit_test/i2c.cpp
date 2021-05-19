// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "i2c.hpp"
#include <cassert>

using namespace test;

i2cTestDriver::~i2cTestDriver() noexcept {}

void i2cTestDriver::start_() noexcept {}

void i2cTestDriver::stop_() noexcept {}

void i2cTestDriver::configure_(embvm::i2c::pullups pullups) noexcept
{
	(void)pullups;
}

embvm::i2c::baud i2cTestDriver::baudrate_(embvm::i2c::baud baud) noexcept
{
	return baud;
}

embvm::i2c::status i2cTestDriver::transfer_(const embvm::i2c::op_t& op,
											const embvm::i2c::master::cb_t& cb) noexcept
{
	switch(op.op)
	{
		case embvm::i2c::operation::stop:
		case embvm::i2c::operation::restart:
		case embvm::i2c::operation::ping:
			bus_status_ = embvm::i2c::status::ok;
			break;
		case embvm::i2c::operation::read:
			assert(op.rx_buffer);
			assert(rxBuffer_.size() >= op.rx_size);
			for(size_t i = 0; i < op.rx_size; i++)
			{
				op.rx_buffer[i] = rxBuffer_.front();
				rxBuffer_.pop();
			}
			break;
		case embvm::i2c::operation::writeRead:
			assert(op.tx_buffer);
			assert(op.tx_size > 0);
			assert(op.rx_buffer);
			assert(rxBuffer_.size() >= op.rx_size);
			for(size_t i = 0; i < op.tx_size; i++)
			{
				txBuffer_.push_back(op.tx_buffer[i]);
			}
			for(size_t i = 0; i < op.rx_size; i++)
			{
				op.rx_buffer[i] = rxBuffer_.front();
				rxBuffer_.pop();
			}
			break;
		case embvm::i2c::operation::write:
		case embvm::i2c::operation::writeNoStop:
		case embvm::i2c::operation::continueWriteNoStop:
		case embvm::i2c::operation::continueWriteStop:
			assert(op.tx_buffer);
			assert(op.tx_size > 0);
			for(size_t i = 0; i < op.tx_size; i++)
			{
				txBuffer_.push_back(op.tx_buffer[i]);
			}
			break;
	}

	// callback(op, embvm::i2c::status::ok, cb);

	return embvm::i2c::status::ok;
}

embvm::i2c::pullups i2cTestDriver::setPullups_(embvm::i2c::pullups pullups) noexcept
{
	return pullups;
}
