# CRC-R: System Power Manager

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* Accepting a variable definition of system power levels
* Enabling users to specify what drivers are supposed to be enabled at a given power level
* Starting/Stopping drivers automatically when a power state change is requested
* Reporting the current power state

## Requirements

* Power management strategy must be configurable by users
* Run levels must be configurable by users

## Collaborators

* The System Power Manager controls [Driver](../core/driver.md) power states directly

## Rationale

Power management is a common requirement in embedded systems, as devices are often in the field operating for long periods of time. Some peripherals only need to be utilized at specific moments.

Users should be able to adjust the power state of their system at runtime using definitions set at compile-time. We can provide a default strategy, but users can override this behavior to suit their system's needs.

## Source Links

TBD

## Related Documents

* [Strategy Pattern](../../../patterns/strategy.md)

## Notes

* We call this a subsystem, but does it belong in the framework core, since it needs to know about device drivers? or can subsystems have platform-specific requirements?

* Concept of run levels with drivers?
    * defines what drivers start/stop in different power states automatically
        * low power, full power, waiting for connection
        * can control what drivers sleep depending on the
    * This model encourages deferred initialization…
* The drivers whose state needs managed register with the pwoer management service
        * unmanged devices don't need to register.
        * user defines custom power states (inherit from base class?)
        * When state changes, registered drivers are updated accordingly
        * Can work directly with driver base class objects only, because just calls start() and stop() generic interfaces
* Could also make the system power manager just be an event vending service that drivers subscribe to.
* What if we make a const std::array or something for each run level?
* Do we ahve the power manager template on a run-level type?
    * e.g., DefaultSystemPowerManager<DefaultRunLevel> spm;
* we want to be able to decouple power levels and power management schemes
* Client can define other actions and state changes, add power functions, send notifications, etc.
* Our default power manager just calls driver start/stop as callbacks based on the power state changes
lowest power state puts the processor to sleep (wfi?)

What is the simplest approach?

```
enum DefaultRunLevel
{
	sleep = 0,
	low = 1,
	standby = 2,
	on = 4
}

```

If we keep power levels as a bitmask, we can set a mask of states. Then we wonder: is there an easy way to automate that bitmask enum definition?

```
register(accel0, 0b1100); // define the states which accel0 should be enabled during -> on + ready
```

What if we make a const std::array or something for each run level? the power manager would need that:

```
low { bluetooth, spi, uart };
standby {accel, spi, bluetooth, uart}
on { accel, spi, bluetooth, uart, camera, usb, wifi }
```

Then we:

```
register(state::low, low)
```

Perhaps we can construct our power manager with a list definition?
