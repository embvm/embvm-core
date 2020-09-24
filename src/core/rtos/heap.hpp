#ifndef RTOS_HEAP_HPP_
#define RTOS_HEAP_HPP_

#include <cassert>
#include <cstddef>

namespace embvm
{
/// @addtogroup FrameworkOS
/// @{

template<typename THeapImpl>
class VirtualHeap
{
  public:
	static void addBlock(void* addr, size_t size) noexcept
	{
		assert(addr && size);

		THeapImpl::addBlock(addr, size);
	}

	static void init() noexcept
	{
		THeapImpl::init();
	}

	static void* alloc(size_t size) noexcept
	{
		return THeapImpl::alloc(size);
	}

	static void free(void* addr) noexcept
	{
		assert(addr);

		return THeapImpl::free(addr);
	}
};

/// @}
// End Group

} // namespace embvm

#endif // RTOS_HEAP_HPP_
