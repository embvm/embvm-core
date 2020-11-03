// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "bounce.hpp"
#include <catch2/catch.hpp>

using namespace embutil;

namespace
{
using func_t = void (*)(void*);

void call_bounce(func_t b, void* priv)
{
	b(priv);
}

class test
{
  public:
	void callback()
	{
		callback_called_ = true;
	}

	bool called()
	{
		return callback_called_;
	}

  private:
	bool callback_called_ = false;
};

} // namespace

TEST_CASE("Bounce", "[utilities/bounce]")
{
	test inst;
	call_bounce(BOUNCE(test, callback), &inst);
	CHECK(inst.called());
}
