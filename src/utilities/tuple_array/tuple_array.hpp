#ifndef TUPLE_ARRAY_HPP_
#define TUPLE_ARRAY_HPP_

#include <array>
#include <tuple>

namespace embutil
{
/** \defgroup Tuple-Array Conversion Utilities
 *
 * Functions that can be used to convert between tuples and arrays.
 *
 * @ingroup FrameworkUtils
 * @{
 */

///@name Tuple-Array Conversion Functions
/// @brief Convert between tuples and arrays

/** The implementation function that handles the actual conversion.
 *
 */
template<typename T, typename Tuple, std::size_t... Indexes>
auto tuple_to_array_impl(Tuple&& tup, std::integer_sequence<std::size_t, Indexes...>)
	-> std::array<T, sizeof...(Indexes)>
{
	return {static_cast<int>(std::get<Indexes>(tup))...};
}

/** Convert a tuple into a std::array
 *
 * This function is the interface used to convert a std::tuple into a std::array.
 *
 * @returns an array containing the tuple elements.
 */
template<typename T, typename... Args>
auto tuple_to_array(std::tuple<Args...>&& tup) -> std::array<T, sizeof...(Args)>
{
	return tuple_to_array_impl<T>(std::move(tup), std::make_index_sequence<sizeof...(Args)>{});
}

/// @}
// End tuple/array conversion module

}; // end namespace embutil

#endif
