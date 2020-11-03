// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include <driver/i2c.hpp>

constexpr uint8_t I2C_ADDR_MAX = 127;

embvm::i2c::master::~master() noexcept = default;

void embvm::i2c::master::sweep(sweep_list_t& found_list, const sweep_cb_t& cb) noexcept
{
	i2c::op_t xact;
	xact.op = i2c::operation::ping;
	embvm::i2c::status status;

	auto callback = [&](i2c::op_t op, i2c::status s) {
		if(s == i2c::status::ok)
		{
			found_list.push_back(op.address);
		}
	};

	for(uint8_t i = 0; i < I2C_ADDR_MAX; i++)
	{
		xact.address = i;
		do
		{
			// TODO: sleep if threading?
			status = transfer(xact, callback);
		} while(status == embvm::i2c::status::busy);
	}

	// The final sweep address will trigger the client callback
	xact.address = I2C_ADDR_MAX;
	do
	{
		// TODO: sleep if threading?
		status = transfer(xact, [&](i2c::op_t op, i2c::status s) {
			if(s == i2c::status::ok)
			{
				found_list.push_back(op.address);
			}

			cb();
		});
	} while(status == embvm::i2c::status::busy);
}
