#ifndef VIRTUAL_PROCESSOR_ARCH_H__
#define VIRTUAL_PROCESSOR_ARCH_H__

namespace embvm
{
/// @addtogroup FrameworkProcessor
/// @{

// TODO: cache add-ons
// TODO: MMU add-ons
// TODO: update architecture documentation

/** Virtual Interface for Common Architecture-Specific Functionality
 *
 * Virtual processor arch won't actually be part of the processor's inheritance tree.
 * There will be a base class, or set of classes, which can be inherited from through CRTP.
 * All methods will be static. There won't be an instance directly.
 * This will allow us to keep things separated much more nicely.
 */
template<typename TArchitecture>
class VirtualProcessorArchBase
{
  public:
#if 0
	static bool areInterruptsEnabled()
	{
		TArchitecture::areInterruptsEnabled_();
	}
#endif

	static void interruptsEnable() noexcept
	{
		TArchitecture::interruptsEnable_();
	}

	static void interruptsDisable() noexcept
	{
		TArchitecture::interruptsDisable_();
	}

	static void memoryBarrier() noexcept
	{
		TArchitecture::memoryBarrier_();
	}

	static void instructionBarrier() noexcept
	{
		TArchitecture::instructionBarrier_();
	}

	/// Deleted copy constructor
	VirtualProcessorArchBase(const VirtualProcessorArchBase&) = delete;

	/// Deleted copy assignment operator
	const VirtualProcessorArchBase& operator=(const VirtualProcessorArchBase&) = delete;

	/// Deleted move constructor
	VirtualProcessorArchBase(VirtualProcessorArchBase&&) = delete;

	/// Deleted move assignment operator
	VirtualProcessorArchBase& operator=(VirtualProcessorArchBase&&) = delete;

  protected:
	~VirtualProcessorArchBase() = default;
	VirtualProcessorArchBase() = default;
};

/// @}
// End group

} // namespace embvm

#endif // VIRTUAL_PROCESSOR_ARCH_H__
