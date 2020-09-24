#include "simulator_processor.hpp"
#include <csignal>

#pragma mark - Definitions -

extern volatile bool abort_program_;

#pragma mark - Helpers -

// This enables graceful termination of our program using CTRL+C
// We put it in this file because only the sim platform variant supports std::signal
namespace
{
void signal_handler(int signal)
{
	(void)signal;
	abort_program_ = true;
}
} // namespace

#pragma mark - Class Functions -

SimulatorProcessor::~SimulatorProcessor() {}

void SimulatorProcessor::init_()
{
	std::signal(SIGINT, signal_handler);
	std::signal(SIGTERM, signal_handler);
}
