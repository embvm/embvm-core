# Adding a New Hardware Platform

* `meson.build`
* `hw_platform.hpp`
* `hw_platform.cpp`

Talk about the helper script once it is created

## Alternatives to Find Driver

```
/** Get the tof0 Instance
*
* You don't always need find_driver() in the DriverRegistry. You can directly return a
* base-class reference from the hardware platform. This saves time from doing a lookup.
*
* It is important to return the base class, since that is how we keep our applications
* decoupled from the underlying hardware. Returning the actual ToF sensor class will
* enable coupling to the underlying harware.
*
* @returns a reference ot the tof0 instance, as the base class type.
*/
embvm::tof::sensor& tof0_inst()
{
	return tof0;
}


/// Elsewhere
	// Instead of using find_driver(), we will get the instance directly
	// from the hardware platform. We use auto& because we are receiving
	// a reference to the base class.
	auto& tof = hw_platform_.tof0_inst();
	tof.registerReadCallback([](uint16_t v) { printf("ToF Range: %u mm\n", v); });

```
