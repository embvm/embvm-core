# Adding a New Driver Type

* Inherit from embvm::DriverBase
* Define a new driver type (in your own type enum, or in the base framework enum)
* Create constructors which pass that type to base
* Define pure virtual interfaces, types, and non-virtual interfaces which your category will support
* Create a static constexpr type() functionw hich returns your new embvm::DriverType ID

```
static constexpr embvm::DriverType type()
{
	return embvm::DriverType::SPI;
}
```

* Create a unit test driver for your virtual base
* Write unit tests for your virtual base
