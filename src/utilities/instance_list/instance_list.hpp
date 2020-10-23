#ifndef INSTANCE_LIST_HPP_
#define INSTANCE_LIST_HPP_

#include <etl/list.h>
#include <etl/vector.h>
#include <list>
#include <string>
#include <type_safe/optional_ref.hpp>
#include <vector>

namespace embutil
{
/** @defgroup InstanceList Instance List
 * @brief Supports tracking instances which can come and go during runtime.
 *
 * @ingroup FrameworkUtils
 * @{
 */

/** Helper struct which is used to store Key/Value pairs in the InstanceList
 *
 * This structure respresents an Instance element. The key and value are stored in the element
 * struct, and an operator==() is implemented to support the find() and find_if() functions.
 *
 * @tparam TTrackedClass the type of class being tracked by the instance element.
 * @tparam TKey the type of the key used to access the element.
 * @related InstanceList
 */
template<class TTrackedClass, typename TKey>
struct InstanceElem
{
	TKey key;
	TTrackedClass* value;

	/** Compare equality between two instance elements
	 *
	 * Both the key and the value must match for two instance elements to be equal.
	 */
	bool operator==(const InstanceElem& t) const noexcept
	{
		return (t.key == this->key && t.value == this->value);
	}
};

/** Generic InstanceList template container
 *
 * This class is not intended to be used directly. Instead use the defined aliases:
 *
 *	- DynamicInstanceList
 *	- StaticInstanceList
 *
 * This list designed to work with STL containers and ETL containers which supply the
 * `push_back()`, `remove()`, and `remove_if()` methods.
 *
 * # Using an InstanceList
 *
 * For an example module which uses an InstanceList, see embvm::DriverBase.
 *
 * The DriverBase InstanceList stores pointers to DriverBase instances, which is the base class
 * for all framework drivers:
 *
 * @code
 * embutil::DynamicInstanceList<embvm::DriverBase, TKey>
 * @endcode
 *
 * When framework drivers are constructed, they are added to the DriverRegistry instance list.
 * The registerDriver() interface forwards the call to the DriverRegistry add() function.
 *
 * @code
 * embvm::DriverBase::DriverBase(const char* name, uint32_t c) : name_(name), type_(c)
 * {
 * 	VirtualPlatform::registerDriver(name_, this);
 * }
 *
 * void DriverRegistry::add(const TKey name, embvm::DriverBase* driver)
 * {
 *	list_.add(name, driver);
 * }
 * @endcode
 *
 * The DeriverBase destructor removes instances from the list. The unregisterDriver() interface
 * forwards the call to the DriverRegistry remove() function.
 *
 * @code
 * embvm::DriverBase::~DriverBase()
 * {
 * 	VirtualPlatform::unregisterDriver(name_, this);
 * }
 *
 * void DriverRegistry::remove(const TKey name, embvm::DriverBase* driver)
 * {
 *	list_.remove(name, driver);
 * }
 * @endcode
 *
 * Lookups can be performed by key:
 *
 * @code
 * auto find(const TKey name)
 * {
 *	return list_.find(name);
 * }
 *
 * The return type of find() is a type_safe::optional_ref<TTrackedClass>. If the value is found,
 * The optional_ref will be valid and can be used. If the value is not found, the optional_ref
 * will be marked as invalid. Validity can be determined with `operator bool()`:
 *
 * @code
 * auto tof = VirtualPlatform::template findDriver<embvm::tof::sensor>();
 * if(tof)
 * {
 * 	tof.value().registerReadCallback([](uint16_t v) { printf("ToF Range: %u mm\n", v); });
 * }
 * else
 * {
 * 	assert(0); // ToF Driver Missing
 * }
 * @endcode
 *
 * @tparam TTrackedClass The type of class which is tracked by this instance list.
 * @tparam TKey The key type which is used to lookup stored values.
 * @tparam TContainer The container type used to store the instance list.
 * @tparam TSize The maximum of elements to track with the list. If TSize is 0, then dynamic
 * memory allocation will be used. Otherwise, static memory allocation is used and the maximum
 * number of elements that can be tracked is TSize.
 */
template<class TTrackedClass, typename TKey, class TContainer, const size_t TSize = 0>
class InstanceList
{
	/// Convenience alias for optional references, used internally to the class.
	using optional_ref = type_safe::optional_ref<TTrackedClass>;

  public:
	using TStorageType = InstanceElem<TTrackedClass, TKey>;

	/// Default constructor
	InstanceList() = default;

	/// Default destructor
	~InstanceList() = default;

	/// Get the current size of the list
	/// @returns the current number of elements in the list.
	size_t size() const noexcept
	{
		return registered_.size();
	}

	/// Get the total storage capacity of the list
	/// @returns the maximum size the InstanceList container can support.
	constexpr size_t capacity() const noexcept
	{
		return registered_.max_size();
	}

	/** Registered instance list accessor
	 *
	 * If you need to perform lookups or other complex operations on the list, but can't work
	 * through the InstanceList interface, you can get the raw storage.
	 *
	 * @returns a reference to the list storage instance
	 */
	TContainer& rawStorage() noexcept
	{
		return registered_;
	}

#pragma mark - Add -

	/// @name Add Instances
	/// @{

	/** Register an instance of the TTrackedClass with a key
	 *
	 * add() should be called in the base-class constructor of the instance type
	 * which you are tracking. See embvm::DriverBase for an example.
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call.
	 *
	 * @param key The key to register the instance under.
	 * @param instance The instance pointer to track.
	 */
	void add(TKey const key, TTrackedClass* const instance) noexcept
	{
		if constexpr(TSize > 0) // NOLINT
		{
			const bool b = (size() < capacity());
			assert(b && "Adding too many values - increase size of static InstanceList");
		}

		registered_.push_back(TStorageType{key, instance}); // NOLINT
	}

	/** Register an instance of the TTrackedClass without a key
	 *
	 * add() should be called in the base-class constructor of the instance type
	 * which you are tracking. See embvm::DriverBase for an example.
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call.
	 *
	 * @param instance The instance pointer to track.
	 */
	void add(TTrackedClass* const instance) noexcept
	{
		add(nullptr, instance);
	}

	/// @}
	// end add instances

#pragma mark - Remove -

	/// @name Remove Instances
	/// @{

	/** Remove instance matching key and value
	 *
	 * Both the key and the value must match for the instance to be removed.
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call.
	 *
	 * @param key The key corresponding to the instance to remove.
	 * @param instance The instance value to remove.
	 */
	void remove(TKey const key, TTrackedClass* const instance) noexcept
	{
		auto inst =
			std::remove(registered_.begin(), registered_.end(), TStorageType{key, instance});
		registered_.erase(inst);
	}

	/** Remove all instances matchey key
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call.
	 *
	 * @param key The corresponding key to remove instances for.
	 */
	void remove(TKey const key) noexcept
	{
		auto insts = std::remove_if(registered_.begin(), registered_.end(),
									[key](TStorageType& inst) { return inst.key == key; });
		registered_.erase(insts);
	}

	/** Remove an instance value from the list
	 *
	 * This function is marked noexcept because we want the program to terminate if an exception
	 * results from this call.
	 *
	 * @param instance The pointer to the instance to remove from the list.
	 */
	void remove(TTrackedClass* const instance) noexcept
	{
		auto insts =
			std::remove_if(registered_.begin(), registered_.end(),
						   [instance](TStorageType& inst) { return inst.value == instance; });
		registered_.erase(insts);
	}

	/// @}
	// End remove instances

#pragma mark - Instance Lookp -

	/// @name Instance Lookup
	/// @{

	/** Indexing operator supports the use of key.
	 *
	 * Allow indexing into this object's key ("compatible-key", "instance-key")
	 *
	 * @param key The key to use for looking up the corresponding TTrackedClass value.
	 * @returns an optional_ref to the TTrackedClass value corresponding to key. If no value is
	 * found that matches the key, the optional_ref will be empty.
	 */
	optional_ref operator[](TKey const key) noexcept
	{
		return find(key);
	}

	/** Find instance using a key
	 *
	 * @param key The key to use for looking up the corresponding TTrackedClass value.
	 * @returns an optional_ref to the TTrackedClass value corresponding to key. If no value is
	 * found that matches the key, the optional_ref will be empty.
	 */
	optional_ref find(TKey const key) noexcept
	{
		TTrackedClass* ptr = nullptr;

		auto val = std::find_if(registered_.begin(), registered_.end(),
								[&](const TStorageType& inst) { return inst.key == key; });

		if(val != registered_.end())
		{
			ptr = val->value;
		}

		return type_safe::opt_ref(ptr);
	}

	/// @}
	// End instance lookup

  private:
	/// The list of registered instances.
	TContainer registered_{};
};

/** Template class to track instances of class-type things (dynamic memory variant)
 *
 * This class supports tracking instances which can come and go during runtime. The
 * DynamicInstanceList uses dynamic memory allocation.
 *
 * Example declarations:
 * @code
 * DynamicInstanceList<embvm::DriverBase> // Dynamic list of driver instances, indexed by string
 * DynamicInstanceList<embvm::DriverBase, uint32_t> // Key is a uint32_t, not a string
 * @endcode
 *
 * @tparam TTrackedClass The type of class which is tracked by this instance list.
 * @tparam TKey The key type which is used to lookup stored values.
 * @tparam TContainer The container type used to store the instance list.
 */
template<class TTrackedClass, typename TKey = const char*,
		 class TContainer = std::list<InstanceElem<TTrackedClass, TKey>>>
using DynamicInstanceList = InstanceList<TTrackedClass, TKey, TContainer>;

/** Template class to track instances of class-type things (static memory variant)
 *
 * This class supports tracking instances which can come and go during runtime. The
 * StaticInstanceList uses static memory allocation.
 *
 * Example declarations:
 * @code
 * StaticInstanceList<embvm::DriverBase> //Fixed list of 32 (default) driver instances, indexed by
 * string StaticInstanceList<embvm::DriverBase, 64> //Specify size as 64
 * StaticInstanceList<embvm::DriverBase, 32, uint32_t> //Key is a uint32_t, not a string
 * @endcode
 *
 * @tparam TTrackedClass The type of class which is tracked by this instance list.
 * @tparam TSize The maximum of elements to track with the list.
 * @tparam TKey The key type which is used to lookup stored values.
 * @tparam TContainer The container type used to store the instance list.
 */
template<class TTrackedClass, const size_t TSize = 32, typename TKey = const char*,
		 class TContainer = etl::list<InstanceElem<TTrackedClass, TKey>, TSize>>
using StaticInstanceList = InstanceList<TTrackedClass, TKey, TContainer, TSize>;

/// @}
// end group

} // namespace embutil

#endif // INSTANCE_LIST_HPP_
