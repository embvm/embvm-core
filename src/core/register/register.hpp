// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef REGISTER_HPP_
#define REGISTER_HPP_

#include <cstdint>
#include <iterator>
#include <type_traits>

namespace embutil
{
/** @defgroup SFR Special Function Register
 *
 * Safer types for working with special-function registers.
 *
 * @ingroup FrameworkUtils
 * @{
 */

/** Register access permissions */
/// Read-write access permission tag.
struct rw
{
};

/// Read-only access permission tag.
struct ro
{
};

/// Write-only access permission tag.
struct wo
{
};

// Forward declare the sfr class and default values
/**
 *
 * @tparam TPerm Tag value indicating the permissions to use for this register. Should be rw, rw,
 *	or wo.
 * @tparam T The type corresponding to the SFR representation (i.e., uint32_t on 32-bit systems).
 * @tparam TAddr The address of the SFR. If the address is 0, it can be assigned during object
 * 	constrution. Otherwise, the address of TAddr is locked in at compile-time.
 * @tparam StaticAddress SFINAE-enabling parameter. You don't need to use this.
 */
template<typename TPerm = rw, typename T = uint32_t, uintptr_t TAddr = 0,
		 typename StaticAddress = void>
class sfr;

/** SFR Class which supports address assignment with a constructor.
 *
 * This variant of the SFR class assigns the register value when the object is created.
 * With an optimizer, this will often result in the same as the static-definition variant.
 */
template<typename TPerm, typename T, uintptr_t TAddr>
class sfr<TPerm, T, TAddr, typename std::enable_if<TAddr == 0>::type>
{
  public:
	// Iterator Expectations
	using iterator_category = std::forward_iterator_tag;
	using value_type = T; // crap
	using difference_type = void;
	using pointer = void;
	using reference = void;

	// Helper types for various return value options
	using ptr_t = volatile T*;
	using const_ptr_t = volatile const T*;
	using ref_t = volatile T&;
	using const_ref_t = volatile const T&;

	/** Construct an SFR with an address
	 *
	 * @param[in] address The address in memory that this SFR points to.
	 */
	explicit sfr(uintptr_t address) noexcept : addr_(address) {}

	/** Construct an SFR with an address and value
	 *
	 * @param[in] address The address in memory that this SFR points to.
	 * @param[in] value The initial value to set.
	 */
	explicit sfr(uintptr_t address, T value) noexcept : addr_(address)
	{
		static_assert(!std::is_same<TPerm, ro>::value,
					  "Cannot assign a value to a read-only register");
		store(value);
	}

	/** Construct an SFR with a pointer
	 *
	 * @param[in] address The pointer that this SFR points to.
	 */
	explicit sfr(T* address) noexcept : addr_(reinterpret_cast<uintptr_t>(address)) {}

	/** Construct an SFR with a pointer and value
	 *
	 * @param[in] address The pointer that this SFR points to.
	 * @param[in] value The initial value to set.
	 */
	explicit sfr(T* address, T value) noexcept : addr_(reinterpret_cast<uintptr_t>(address))
	{
		static_assert(!std::is_same<TPerm, ro>::value,
					  "Cannot assign a value to a read-only register");
		store(value);
	}

	/// Delete the copy constructor
	sfr(const sfr&) = delete;

	/// Delete the copy assignment operator
	const sfr& operator=(const sfr&) = delete;

	sfr(sfr&&) = default;
	sfr& operator=(sfr&&) = default;

	/// Default destructor
	~sfr() = default;

	/// Get the address of this SFR
	/// @returns the address of the SFR.
	constexpr uintptr_t address() const noexcept
	{
		return addr_;
	}

	/// Read from register, explicit operation
	/// @returns the current value of the SFR.
	inline T load() const noexcept
	{
		static_assert(!std::is_same<TPerm, wo>::value, "Invalid read to a write-only register");
		return *(reinterpret_cast<const_ptr_t>(addr_));
	}

	/// Read from register, read-only dereference
	/// @returns the current value of the SFR.
	operator T() const noexcept
	{
		static_assert(!std::is_same<TPerm, wo>::value, "Invalid read to a write-only register");
		return *(reinterpret_cast<const_ptr_t>(addr_));
	}

	/// Read from register
	/// @returns the current value of the SFR.
	operator T() noexcept
	{
		static_assert(!std::is_same<TPerm, wo>::value, "Invalid read to a write-only register");
		return *(reinterpret_cast<ptr_t>(addr_));
	}

	/// Write to the register
	/// @param[in] value The value to write to the SFR.
	inline void store(T value) noexcept
	{
		static_assert(!std::is_same<TPerm, ro>::value, "Invalid write to a read-only register");
		*(reinterpret_cast<ptr_t>(addr_)) = value;
	}

	/// Write via copy/assignment
	/// @param[in] value_ The value to write to the SFR.
	sfr& operator=(T value_) noexcept
	{
		static_assert(!std::is_same<TPerm, ro>::value, "Invalid write to a read-only register");
		*(reinterpret_cast<ptr_t>(addr_)) = value_;
		return *this;
	}

	/// Read/write dereference
	ref_t operator*() noexcept
	{
		return *(reinterpret_cast<ptr_t>(addr_));
	}

	/// Read-only dereference
	const_ref_t operator*() const noexcept
	{
		static_assert(!std::is_same<TPerm, wo>::value, "Invalid read to a write-only register");
		return *(reinterpret_cast<const_ptr_t>(addr_));
	}

	/// Increment operator is a no-op, but is used for iterator compatibility
	sfr& operator++() noexcept
	{
		return *this;
	}

	/// Increment operator is a no-op, but is used for iterator compatibility
	sfr operator++(int) noexcept
	{
		return *this;
	}

  protected:
	/// The address of the SFR
	const uintptr_t addr_;
};

/// This variant of the SFR class is used when the address is known at compile-time
template<typename TPerm, typename T, uintptr_t TAddr>
class sfr<TPerm, T, TAddr, typename std::enable_if_t<TAddr != 0, void>>
{
  public:
	// Iterator Expectations
	using iterator_category = std::forward_iterator_tag;
	using value_type = T; // crap
	using difference_type = void;
	using pointer = void;
	using reference = void;

	// Helper types for various return value options
	using ptr_t = volatile T*;
	using const_ptr_t = volatile const T*;
	using ref_t = volatile T&;
	using const_ref_t = volatile const T&;

	/// Default constructor
	sfr() = default;

	/** Construct an SFR and set a value
	 *
	 * @param[in] value The initial value to set.
	 */
	explicit sfr(T value) noexcept
	{
		static_assert(!std::is_same<TPerm, ro>::value,
					  "Cannot assign a value to a read-only register");
		store(value);
	}

	/// Default destructor
	~sfr() = default;

	/// Delete the copy constructor
	sfr(const sfr&) = delete;

	/// Delete the copy assignment operator
	const sfr& operator=(const sfr&) = delete;

	sfr(sfr&&) = default;
	sfr& operator=(sfr&&) = default;

	/// Get the address of this SFR
	/// @returns the address of the SFR.
	constexpr uintptr_t address() const noexcept
	{
		return TAddr;
	}

	/// Read from register, explicit operation
	/// @returns the current value of the SFR.
	inline T load() const noexcept
	{
		static_assert(!std::is_same<TPerm, wo>::value, "Invalid read to a write-only register");
		return *(reinterpret_cast<const_ptr_t>(TAddr));
	}

	/// Read from register, read-only dereference
	/// @returns the current value of the SFR.
	operator T() const noexcept
	{
		static_assert(!std::is_same<TPerm, wo>::value, "Invalid read to a write-only register");
		return *(reinterpret_cast<const_ptr_t>(TAddr));
	}

	/// Read from register
	/// @returns the current value of the SFR.
	operator T() noexcept
	{
		static_assert(!std::is_same<TPerm, wo>::value, "Invalid read to a write-only register");
		return *(reinterpret_cast<ptr_t>(TAddr));
	}

	/// Write to the register
	/// @param[in] value The value to write to the SFR.
	inline void store(T value) noexcept
	{
		static_assert(!std::is_same<TPerm, ro>::value, "Invalid write to a read-only register");
		*(reinterpret_cast<ptr_t>(TAddr)) = value;
	}

	/// Write via copy/assignment
	/// @param[in] value_ The value to write to the SFR.
	sfr& operator=(T value_) noexcept
	{
		static_assert(!std::is_same<TPerm, ro>::value, "Invalid write to a read-only register");
		*(reinterpret_cast<ptr_t>(TAddr)) = value_;
		return *this;
	}

	/// Read/write dereference
	ref_t operator*() noexcept
	{
		return *(reinterpret_cast<ptr_t>(TAddr));
	}

	/// Read-only dereference
	const_ref_t operator*() const noexcept
	{
		static_assert(!std::is_same<TPerm, wo>::value, "Invalid read to a write-only register");
		return *(reinterpret_cast<const_ptr_t>(TAddr));
	}

	/// Increment operator is a no-op, but is used for iterator compatibility
	sfr& operator++() noexcept
	{
		return *this;
	}

	/// Increment operator is a no-op, but is used for iterator compatibility
	sfr operator++(int) noexcept
	{
		return *this;
	}
};

/// @}
// End group

} // namespace embutil

#endif // REGISTER_HPP_
