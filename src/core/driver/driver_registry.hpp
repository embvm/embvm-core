#ifndef DRIVER_REGISTRY_HPP_
#define DRIVER_REGISTRY_HPP_

#include "driver_type.hpp"
#include <instance_list/instance_list.hpp>
#include <nop_lock/nop_lock.hpp>

/* Improvement Ideas
 *
 * Remove by name
 * Remove by pointer to embvm::DriverBase
 */

namespace embvm
{
/// @addtogroup FrameworkDriver
/// @{

// Forward declaration of Driver Base Class, defined in driver.hpp
class DriverBase;

/** DriverRegistry base class
 *
 * DriverRegistry provides a common DriverRegistry interface that supports
 * both static and dynamic memory allocation styles. Whether dynamic or static memory
 * is used is determined at compile-time by the template arguments.
 *
 * Two convenience definitions are supplied below: `DynamicDriverRegistry` and
 *`StaticDriverRegistry<>` These types should be used to declare DriverRegistry instances for your
 *platform rather than directly using DriverRegistry. They are more expressive and will be
 *easier for readers to understand.
 *
 * Only one DriverRegistry should be defined for each platform & program.
 *
 * @tparam TMaxSize specifies the maximum number of drivers that can be stored.
 *	A value of '0' indicates that dynamic memory should be used.
 * @tparam TReturnSize specifies the maximum number of drivers that can be returned by find_all()
 *	A value of '0' indicates no limit (dynamic memory)
 * @tparam TLockType The type of lock to use with DriverRegistry. Defaults to embutil::nop_lock,
 *	which does not perform any actual locking. If you are using this DriverRegistry in a
 *multi-threaded program and are worried about locking, you can change this to another type such as
 *std::mutex or an interrupt_lock.
 */
template<const size_t TMaxSize, const size_t TReturnSize, typename TLockType = embutil::nop_lock>
class DriverRegistry
{
	using TKey = const char*;
	using TListType = typename std::conditional<
		(TMaxSize == 0), embutil::DynamicInstanceList<embvm::DriverBase, TKey>,
		embutil::StaticInstanceList<embvm::DriverBase, TMaxSize, TKey>>::type;

  public:
	/// Default constructor
	DriverRegistry() = default;

	/// Default destructor
	~DriverRegistry() noexcept = default;

	/// Deleted copy constructor
	DriverRegistry(const DriverRegistry&) = delete;

	/// Deleted copy assignment operator
	const DriverRegistry& operator=(const DriverRegistry&) = delete;

	/// Deleted move constructor
	DriverRegistry(DriverRegistry&&) = delete;

	/// Deleted move assignment operator
	DriverRegistry& operator=(DriverRegistry&&) = delete;

	/** Get the registered driver count.
	 *
	 * @returns the number of drivers currently registered.
	 */
	size_t count() const noexcept
	{
		return list_.size();
	}

	/** Get the capacity of the registry.
	 *
	 * The capacity represents the total number of drivers which may be registered.
	 *
	 * @returns the capacity of the driver registry.
	 */
	size_t capacity() const noexcept
	{
		return list_.capacity();
	}

	/** Register a driver.
	 *
	 * Adds a driver to the DriverRegistry.
	 *
	 * @param name The name of the driver instance being added.
	 *	Name is used as a key during lookups.
	 * @param driver Pointer to the embvm::DriverBase object. A pointer is used because
	 *	there are any number of potential derived classes which will be tracked.
	 *	To prevent slicing, a pointer to the base class is stored.
	 */
	void add(const TKey name, embvm::DriverBase* driver) noexcept
	{
		lock_.lock();
		list_.add(name, driver);
		lock_.unlock();
	}
	// Dev note: I'd prefer to call this function "register", but it's "add"
	// because "register" is a C/C++ keyword...

	/** Unregister a driver.
	 *
	 * Removes a driver from the DriverRegistry.
	 *
	 * @param name The name of the driver instance being removed.
	 * @param driver Pointer to the embvm::DriverBase object being removed.
	 */
	void remove(const TKey name, embvm::DriverBase* driver) noexcept
	{
		lock_.lock();
		list_.remove(name, driver);
		lock_.unlock();
	}
	// Dev note: I'd prefer to call this function "unregister", but it's "remove"
	// for symmetry with "add"

	/** Find a driver by name.
	 *
	 * Performs a driver lookup by name. If the driver is found, the embvm::DriverBase for that
	 *object will be returned to the user. If no driver is found, an empty object will be returned.
	 *
	 * If multiple objects are registered with the same name, the first one found will
	 * be returned.
	 *
	 * @param name The name of the driver instance to search for.
	 * @returns an type_safe::optional_ref to the embvm::DriverBase object. If the driver
	 *	was not found, the optional_ref will be empty. The caller must cast to the appropriate type.
	 */
	auto find(const TKey name) noexcept
	{
		return list_.find(name);
	}

	/** Find a driver by type.
	 *
	 * Performs a driver lookup by type. If the driver is found, the embvm::DriverBase for that
	 *object will be returned to the user. If no driver is found, an empty object will be returned.
	 *
	 * If multiple objects are registered with the same type, the first one found will
	 * be returned.
	 *
	 * @param dtype The type of the driver instance to search for.
	 * @returns an type_safe::optional_ref to the embvm::DriverBase object. If the driver
	 *	was not found, the optional_ref will be empty.
	 */
	auto find(DriverType_t dtype) noexcept
	{
		embvm::DriverBase* ptr = nullptr;

		// This breaks encapsulation for IteratorList - but is simplest for now
		auto list = list_.rawStorage();

		auto val = std::find_if(list.begin(), list.end(), [&](const auto& inst) {
			return inst.value->DriverType() == dtype;
		});

		if(val != list.end())
		{
			ptr = val->value;
		}

		return type_safe::opt_ref(ptr);
	}

	/** Find a driver by type and return casted interface pointer.
	 *
	 * Performs a driver lookup by type. If the driver is found, the embvm::DriverBase will be
	 *casted to the requested driver interface. If no driver is found, an empty object will be
	 *returned.
	 *
	 * To perform the lookup, the requested driver classes's `::type()` static member
	 * function will be used.
	 *
	 * If multiple objects are registered with the same type, the first one found will
	 * be returned.
	 *
	 * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
	 * @returns an type_safe::optional_ref to the embvm::DriverBase object. If the driver
	 *	was not found, the optional_ref will be empty.
	 */
	template<class TDriverClass>
	auto find() noexcept
	{
		auto ptr = find(TDriverClass::type());

		return type_safe::opt_ref(static_cast<TDriverClass*>(ptr ? &ptr.value() : nullptr));
	}

	/** Find a driver by name and return casted interface pointer.
	 *
	 * Performs a driver lookup by name. If the driver is found, the embvm::DriverBase will be
	 *casted to the requested driver interface. If no driver is found, an empty object will be
	 *returned.
	 *
	 * If multiple objects are registered with the same name, the first one found will
	 * be returned.
	 *
	 * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
	 * @param name The name of the driver instance to search for.
	 * @returns an type_safe::optional_ref to the embvm::DriverBase object. If the driver
	 *	was not found, the optional_ref will be empty.
	 */
	template<class TDriverClass>
	auto find(const TKey name) noexcept
	{
		auto ptr = find(name);

		return type_safe::opt_ref(static_cast<TDriverClass*>(ptr ? &ptr.value() : nullptr));
	}

	/** Find all drivers with a given type (dynamic memory).
	 *
	 * Performs a driver lookup by type and creates a list of all drivers found with the matching
	 * type. If no driver is found, an empty list will be returned.
	 *
	 * @tparam TReturn The return type provided to the caller. Most users do not need to change the
	 * type.
	 * @param dtype The type of the driver instance to search for.
	 * @returns a TReturn type (default std::list) of pointers to Dembvm::DriverBase objects with
	 * the requested driver type. An empty list will be returned if no drivers are found.
	 */
	template<typename TReturn = std::list<embvm::DriverBase*>>
	auto find_all(DriverType_t dtype) noexcept -> std::enable_if_t<!TReturnSize, TReturn>
	{
		TReturn found_list;

		// This breaks encapsulation for IteratorList - but is simplest for now
		for(const auto& t : list_.rawStorage())
		{
			if(t.value->DriverType() == dtype)
			{
				found_list.push_back(t.value);
			}
		}

		return found_list;
	}

	/** Find all drivers with a given type (static memory).
	 *
	 * Performs a driver lookup by type and creates a list of all drivers found with the matching
	 * type. If no driver is found, an empty list will be returned.
	 *
	 * @tparam TReturn The return type provided to the caller. Most users do not need to change the
	 * type.
	 * @param dtype The type of the driver instance to search for.
	 * @returns a TReturn type (default etl::list) of pointers to Dembvm::DriverBase objects with
	 * the requested driver type. An empty list will be returned if no drivers are found.
	 */
	template<typename TReturn = etl::list<embvm::DriverBase*, TReturnSize>>
	auto find_all(DriverType_t dtype) noexcept -> std::enable_if_t<(TReturnSize > 0), TReturn>
	{
		TReturn found_list;

		// This breaks encapsulation for IteratorList - but is simplest for now
		for(const auto& t : list_.rawStorage())
		{
			if(t.value->DriverType() == dtype)
			{
				found_list.push_back(t.value);
			}
		}

		return found_list;
	}

	/** Find all drivers with a given type and return casted interface pointers (dynamic memory).
	 *
	 * Performs a driver lookup by type and creates a list of all drivers found with the matching
	 * type. If no driver is found, an empty list will be returned.
	 *
	 * To perform the lookup, the requested driver classes's `::type()` static member
	 * function will be used.
	 *
	 * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
	 * @tparam TReturn The return type provided to the caller. Most users do not need to change the
	 * type.
	 * @returns a TReturn type (default std::list) of pointers to Dembvm::DriverBase objects with
	 * the requested driver type. An empty list will be returned if no drivers are found.
	 */
	template<class TDriverClass, typename TReturn = std::list<embvm::DriverBase*>>
	auto find_all() noexcept -> std::enable_if_t<!TReturnSize, TReturn>
	{
		TReturn found_list;

		// This breaks encapsulation for IteratorList - but is simplest for now
		for(const auto& t : list_.rawStorage())
		{
			if(t.value->DriverType() == TDriverClass::type())
			{
				found_list.push_back(t.value);
			}
		}

		return found_list;
	}

	/** Find all drivers with a given type and return casted interface pointers (static memory).
	 *
	 * Performs a driver lookup by type and creates a list of all drivers found with the matching
	 * type. If no driver is found, an empty list will be returned.
	 *
	 * To perform the lookup, the requested driver classes's `::type()` static member
	 * function will be used.
	 *
	 * @tparam TDriverClass The class of driver being requested (embvm::i2c::master, SystemClock).
	 * @tparam TReturn The return type provided to the caller. Most users do not need to change the
	 * type.
	 * @returns a TReturn type (default etl::list) of pointers to Dembvm::DriverBase objects with
	 * the requested driver type. An empty list will be returned if no drivers are found.
	 */
	template<class TDriverClass, typename TReturn = etl::list<embvm::DriverBase*, TReturnSize>>
	auto find_all() noexcept -> std::enable_if_t<(TReturnSize > 0), TReturn>
	{
		TReturn found_list;

		// This breaks encapsulation for IteratorList - but is simplest for now
		for(const auto& t : list_.rawStorage())
		{
			if(t.value->DriverType() == TDriverClass::type())
			{
				found_list.push_back(t.value);
			}
		}

		return found_list;
	}

  private:
	TListType list_{};
	TLockType lock_{};
};

/** Declare a DriverRegistry that uses dynamic memory allocation.
 *
 * A DynamicDriverRegistry utilizes dynamic memory allocation and can grow to the limit that memory
 * allows. This type is useful for host-based simulation platforms, as well as embedded platforms
 * which allow dynamic memory allocation.
 *
 * @tparam TLockType The type of lock to use with DynamicDriverRegistry. See DriverRegistry for more
 * information.
 */
template<typename TLockType = embutil::nop_lock>
using DynamicDriverRegistry = DriverRegistry<0, 0, TLockType>;

/** Declare a DriverRegistry that uses static memory allocation.
 *
 * The size and memory allocation of the StaticDriverRegistry is known at compile-time.
 * No dynamic memory allocations are used.
 *
 * In addition, the number of drivers returned by the find_all() function is fixed.
 *
 * @tparam TMaxSize specifies the maximum number of drivers that can be stored
 * @tparam TReturnSize specifies the maximum number of drivers that can be returned by find_all()
 * @tparam TLockType The type of lock to use with StaticDriverRegistry. See DriverRegistry for more
 * information.
 */
template<const size_t TMaxSize = 32, const size_t TReturnSize = 4,
		 typename TLockType = embutil::nop_lock>
using StaticDriverRegistry = DriverRegistry<TMaxSize, TReturnSize, TLockType>;

/// @}
// End group

} // namespace embvm

#endif // DRIVER_REGISTRY_HPP_
