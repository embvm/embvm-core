#ifndef SIMULATOR_PROCESSOR_HPP_
#define SIMULATOR_PROCESSOR_HPP_

#include <processor/virtual_processor.hpp>

class SimulatorProcessor : public embvm::VirtualProcessorBase<SimulatorProcessor>
{
	using ProcessorBase = embvm::VirtualProcessorBase<SimulatorProcessor>;

  public:
	/// @brief Default constructor.
	SimulatorProcessor() : ProcessorBase("Simulator Processor") {}

	/// @brief Default destructor.
	~SimulatorProcessor();

#pragma mark - Inherited Functions -

	static void earlyInitHook_() {}

	void init_();

	void reset_() {}

#pragma mark - Custom Functions -

  private:
  private:
};

#endif // SIMULATOR_PROCESSOR_HPP_
