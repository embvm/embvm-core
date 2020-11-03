// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef SIMULATOR_PROCESSOR_HPP_
#define SIMULATOR_PROCESSOR_HPP_

#include <processor/virtual_processor.hpp>

class SimulatorProcessor : public embvm::VirtualProcessorBase<SimulatorProcessor>
{
	using ProcessorBase = embvm::VirtualProcessorBase<SimulatorProcessor>;

  public:
	/// @brief Default constructor.
	SimulatorProcessor() noexcept = default;

	/// @brief Default destructor.
	~SimulatorProcessor() noexcept = default;

#pragma mark - Inherited Functions -

	static void earlyInitHook_() {}

	void init_();

	void reset_() {}
};

#endif // SIMULATOR_PROCESSOR_HPP_
