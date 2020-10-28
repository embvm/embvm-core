## What is the Embedded Virtual Machine?


## Example Projects

We provide a number of example projects within the [`embvm` GitHub organization](https://github.com/embvm):

- [embvm/embvm-demo-platforms](https://github.com/embvm/embvm-demo-platforms) contains example platforms and hardware platforms that are shared by our [blinky](https://github.com/embvm/blinky) and [embvm-demo](https://github.com/embvm/embvm-demo) projects
- [embvm/blinky](https://github.com/embvm/blinky) contains the application layer code for a simple `blinky` example. This project runs on a personal computer using an Aardvark adapter hooked up to an LED, an nRF52840 development kit, an nRF USB Dongle, and an STM32 NUCLEO-L4R5ZI development board.
- [embvm/embvm-demo](https://github.com/embvm/embvm-demo) contains a more complicated example application involving LEDs, a time-of-flight sensor, and an OLED display. This application runs on a personal computer using an Aardvark adapter and an nRF52840 development kit.
- [embvm/stm32l4](https://github.com/embvm/stm32l4) provides an example hardware platform, platform, and blinky application that will run on an STM32 NUCLEO-L4R5ZI development kit.

## Modules and Drivers

[Modules](embvm_modules.md)
[Drivers](drivers.md)

## Ready to Dive In?

If you're ready to dive in, we recommend starting with our [New Project Guide](new_project_guide), which will walk you through all of the steps needed to get your own `embvm` project up and running.
