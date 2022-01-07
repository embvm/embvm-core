// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#ifndef TIME_OF_FLIGHT_HPP_
#define TIME_OF_FLIGHT_HPP_

#include <driver/driver.hpp>
#include <inplace_function/inplace_function.hpp>
#include <type_safe/optional.hpp>

namespace embvm
{
/// @addtogroup FrameworkDriver
/// @{

/// Definitions, functions, and classes related to Time-of-Flight sensors.
namespace tof
{
/// Representation for the ToF range result in mm.
using distance_t = uint16_t;

/// ToF Callback function which retuns distance in mm
using cb_t = stdext::inplace_function<void(distance_t)>;

/// Representation for an invalid range result.
static const distance_t INVALID_RANGE = UINT16_MAX;

/** ToF ranging mode options.
 *
 * This enum represents the different ToF ranging settings. Not every sensor
 * will have all of these modes; invalid modes should assert() or default
 * to another mode.
 */
enum class mode
{
	/// The sensor's default ranging setting.
	defaultRange = 0,
	/// Short-range mode.
	shortRange,
	/// Medium-range mode.
	medRange,
	/// Long-range mode.
	longRange,
};

enum class status
{
	/// Reading is valid.
	ok = 0,
	/// Measurement did not change.
	noChange,
	/// Measurement was not ready.
	notReady,
	/// Target is below min detection threshold.
	rangeValidMinRangeClipped,
	/// Range is OK but object is a result of multiple pulses merged together.
	rangeValidMergedPulse,
	/// Range is invalid.
	rangeInvalid,
	/// There was an unexpectedly large jump between signal readings.
	sigmaError,
	/// An error identifying or interpreting the signal occurred.
	signalError,
	/// Phase out of valid limits.
	outOfBoundsError,
	/// An error occurred with the sensor hardware.
	hardwareError,
	/// A sensor internal algorithm Errored.
	processingError,
	/// Crosstalk resulted in an invalid signal.
	crosstalkSignalError,
	/// The ROI setting provided to the sensor was invalid.
	invalidRoi,
	/// An unknown error occurred.
	unknown
};

/** This class defines common time-of-flight (ToF) sensor interfaces.
 *
 * tof::sensor is the base class for time-of-flight (ToF) sensor devices.
 *
 * # Define a ToF Driver
 *
 * To define a ToF driver, derive from the tof::sensor object:
 *
 * @code
 * class vl53l1x final : public tof::sensor
 * { ... };
 * @endcode
 *
 * Derived classes must implement the following functions:
 * - read()
 * - reset()
 * - registerReadCallback()
 * - mode()
 * - getMaxRangeForModeDark()
 * - getMaxRangeForModeStrongLight()
 *
 * Additional sensor-specific functions may be defined. They are simply not available in the
 * public/common interface.
 *
 * Derived classes must also implement pure virtual embvm::DriverBase functions:
 * - start_()
 * - stop_()
 *
 * # Using a ToF driver
 *
 * The ToF driver should be configured with the desired ranging mode (tof::mode) using the mode()
 * function.
 *
 * Once the sensor is configured, a callback should be registered via registerReadCallback().
 * The callback is required in order to receive results from the read() command.
 *
 * To get a range result from the sensor, call the read() function. The function operates
 * asynchronously and will return immediately. When the read operation completes, registered
 * callbacks will be called.
 */
class sensor : public embvm::DriverBase
{
  protected:
	/** Default constructor.
	 *
	 * Initializes the base class with the appropriate typeid
	 */
	sensor() noexcept : embvm::DriverBase(embvm::DriverType::TIME_OF_FLIGHT)
	{
		// empty constructor
	}

	/// Default destructor.
	~sensor() noexcept;

  public:
	/** ToF Driver Type ID
	 *
	 * @returns ToF type ID.
	 */
	static constexpr auto type() noexcept -> embvm::DriverType
	{
		return embvm::DriverType::TIME_OF_FLIGHT;
	}

	/** Check the maximum range in the dark.
	 *
	 * @returns the sensor's maximum distance capability (in mm) in the dark.
	 */
	[[nodiscard]] virtual tof::distance_t getMaxRangeForModeDark(tof::mode m) const noexcept = 0;

	/** Check the maximum range in strong light conditions.
	 *
	 * @returns the sensor's maximum distance capability (in mm) in strong light.
	 */
	[[nodiscard]] virtual tof::distance_t
		getMaxRangeForModeStrongLight(tof::mode m) const noexcept = 0;

	/** Get the ToF ranging mode.
	 *
	 * @returns the currently configured ToF ranging mode.
	 */
	[[nodiscard]] auto mode() const noexcept -> tof::mode
	{
		return mode_;
	}

	/** Set the ToF ranging mode.
	 *
	 * @param m The desired ToF ranging mode.
	 * @returns the active ToF mode.
	 */
	virtual auto mode(tof::mode m) noexcept -> tof::mode = 0;

	/** Register a callback for the read() function.
	 *
	 * The read() function works asynchronously, and the result will be provided
	 * to consumers through a callback function. When the read() operation completes,
	 * the callback will be invoked with the result.
	 *
	 * This function must be implemented by the derived class.
	 *
	 * @param cb The functor which will be called when read() completes.
	 */
	virtual void registerReadCallback(const cb_t& cb) noexcept = 0;

	/** Register a callback for the read() function.
	 *
	 * The read() function works asynchronously, and the result will be provided
	 * to consumers through a callback function. When the read() operation completes,
	 * the callback will be invoked with the result.
	 *
	 * This function must be implemented by the derived class.
	 *
	 * @param cb The functor which will be called when read() completes.
	 */
	virtual void registerReadCallback(cb_t&& cb) noexcept = 0;

	/** Trigger a sensor read.
	 *
	 * Trigger an asynchronous read of the ToF sensor. The result will be provided
	 * to consumers through a callback function. When the read() operation completes,
	 * the callback will be invoked with the result.
	 */
	virtual void read() noexcept = 0;

	/// Reset the ToF sensor.
	virtual void reset() noexcept = 0;

  protected:
	// embvm::DriverBase function that derived classes must implement.
	void start_() noexcept override = 0;

	// embvm::DriverBase function that derived classes must implement.
	void stop_() noexcept override = 0;

  protected:
	/// The configured ranging mode for the ToF sensor.
	tof::mode mode_ = tof::mode::defaultRange;
};

} // namespace tof

/// @}
// End group

} // namespace embvm

#endif // TIME_OF_FLIGHT_HPP_
