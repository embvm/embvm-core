# Embedded Virtual Machine Module Guide

The Embedded Virtual Machine project comprises of a core project that can be extended with additional modules.

We categories [drivers](drivers.md) separately from the modules described below, primarily for organizational reasons.

## Operating Systems

The `embvm-core` project provides a portable OS API that can be used to decouple your application from an underlying RTOS. Within the core project, we provide an implementation for the POSIX API, which can be used for building portable applications while starting on your personal computer. 

Additional RTOS support is provided by modules:

* [embvm/freertos-interface](https://github.com/embvm/freertos-interface)
    - Contains the FreeRTOS implementation for the OS API
    - Note that due to the design of FreeRTOS, you will still need to implement the port and configuration settings for your target processor. We supply default implementations for Embedded VM processor modules.

ThreadX support is coming in the near future.

## Processor Architecture

* [embvm/arm-architecture](https://github.com/embvm/arm-architecture)
    - Provides common functionality that can be used across ARM processor, such as an NVIC interface and ARM/Thumb "interrupt locks". 
    - This project is used by our ARM processor modules, described below.

## Processor Support

We currently provide two processor support modules for the Embedded VM ecosystem. Both are currently a work-in-progress:

* [embvm/nordic](https://github.com/embvm/nordic)
    - Support for Nordic processors; currently targeted at minimal support for an nRF52840, with support expanding in the near future.
    - Currently supported drivers:
        + GPIO
        + I2C Master
* [embvm/stm32l4](https://github.com/embvm/nordic)
    - Support for STM32L4 processor family; currently targeted at support for STM32L4R5xx processors, with support expanding in the near future.
    - Currently supported drivers:
        + GPIO
        + DMA
        + Timer (in Timer Base mode)
        + I2C Master (DMA + Interrupts only)
    - Contains a minimal hardware platform, platform, and blinky application that work on an STM32L4R5 Nucleo board

Please file an issue if you are looking for additional driver or processor support, and we will make a plan for completing your request.
