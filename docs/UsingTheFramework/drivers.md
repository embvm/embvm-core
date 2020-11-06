# Embedded VM Drivers

We provide access to device drivers that we've written for use with the Embedded VM ecosystem in the [embvm-drivers](https://github.com/embvm-drivers/) GitHub organization.

## Non-Embedded Drivers

Some device drivers are written to provide support with "simulator" applications, modules, and drivers that are developed on your personal computer.

* [embvm-drivers/aardvark](https://github.com/embvm-drivers/aardvark)
    - Adds support for using the Total Phase Aardvark I2C/SPI/GPIO debug adapter as Embedded VM drivers.
    - We use this project to develop and test I2C/SPI peripheral drivers on our build machine. Once finished, we can then move the peripheral driver to the target hardware without requiring any modification.

## Peripheral Drivers

* [embvm-drivers/ST-VL53L1X](https://github.com/embvm-drivers/ST-VL53L1X)
    - Driver for the ST VL53L1X Time-of-Flight sensor. 
    - Currently a work in progress; this driver operates asynchronously, but needs to be redesigned to be less confusing.
* [embvm-drivers/two-tone-oled](https://github.com/embvm-drivers/two-tone-oled)
    - OLED driver implementation for two-tone OLED displays.
    - Currently a work in progress - it supports the SSD1306 display driver and a fixed size, but we will be refactoring the driver to work in a more generalized manner.

## Adding New Drivers

You are not limited to using the drivers that we provide - you can create your own private drivers using the `embvm-core` driver abstract interfaces. You can also create your own abstract interfaces that are used exclusively within your ecosystem.

For more information on developing Embedded VM device drivers please, see our guide for [Adding a New Driver](adding_new_driver.md). If you want to define your own interface, or add a new interface, please see our guide for [Adding a New Driver Type](adding_new_driver_type.md).
