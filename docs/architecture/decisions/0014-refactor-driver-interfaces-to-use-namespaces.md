# 14. Refactor driver interfaces to use namespaces

Date: 2019-02-20

## Status

Accepted

## Context

Driver interfaces were previously defined in the global namespace, and associated types were defined as `struct`s in the global namespace with a generic name like `tof`:

```
struct tof
{
	 using distance_t = uint16_t;
	 using cb_t = stdext::inplace_function<void(distance_t)>;
	 static const distance_t INVALID_RANGE = UINT16_MAX;
	 enum class mode
	 {
	         defaultRange = 0,
	         shortRange,
	         medRange,
	         longRange,
	 };
}''
```

Interface classes would inherit from these structs:

```
class TimeOfFlight : public embvm::DriverBase, public embvm::tof
```

And the effect was similar to namespacing (`embvm::tof::mode`):

```
virtual embvm::tof::mode mode(embvm::tof::mode m) = 0;
```

Essentially, we are recreating a feature that `namespace` already provides and complicating our inheritance chains.

## Decision

Each driver interface class and any related types are to be stored in a separate namespace per driver type. This keeps related types tied together, improves our generated documentation, and provides improved names for classes and types.

## Consequences

Names for common concepts can be reused across driver implementations. For example, class names such as `i2cMaster` or `spiMaster` are now refactored to be `embvm::i2c::master` or `embvm::spi::master`.

Users must manually specify namespaces in the program. This can be cumbersome to type and remember everything.

For example, the `SystemClock` usage:

```
class SimulatorSystemClock final : public SystemClock
```

Is now namespaced to `embvm::clk::SystemClock`

```
class SimulatorSystemClock final : public embvm::clk::SystemClock
```


`using namespace` helps alleviate the namespace burden, but is not recommended for headers (which most of our files are). However, the use of explicit namespaces within the framewor itself will also serve as a resource/example for other developers.
