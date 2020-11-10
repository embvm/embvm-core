# Getting to Blinky

It's always good to build a simple application to make sure that things are working properly. We like to use the traditional blinking LED for that.

Reference other things:

- bringing up a new processor, new platform, new hardware platform
- OR importing an existing processor module.


Define / create a blinky driver class
add an LED wrapper
create a timer driver or a thread to handle blinking
put together a linker script



### Setting up the Hardware Platform

In the hardware platform header file, we can include the `stm32_gpio.hpp` header and declare some GPIO pins:
    
```
    STM32GPIOOutput<STM32GPIOPort::C, 7> led1_pin;
    STM32GPIOOutput<STM32GPIOPort::B, 7> led2_pin;
    STM32GPIOOutput<STM32GPIOPort::B, 14> led3_pin;
```

Next, we'll declare LED drivers that map to our GPIO pins:

```
    embvm::led::gpioActiveHigh led1{led1_pin, "led1"};
    embvm::led::gpioActiveHigh led2{led2_pin, "led2"};
    embvm::led::gpioActiveHigh led3{led3_pin, "led3"};
```

In our `.cpp` file, we need to actually start the drivers and use them in our code.

```
void NucleoL4R5ZI_HWPlatform::init_() noexcept
{
    // start all LEDs
    // turn them off? Or just trust that they start off?
    led1.start();
    led2.start();
    led3.start();
    led4.start();
}

void NucleoL4R5ZI_HWPlatform::leds_off() noexcept {
    led1.off();
    led2.off();
    led3.off();
}

For our start_blink implementation, we'll just turn them on for now so it's an obvious sign

void NucleoL4R5ZI_HWPlatform::startBlink() noexcept
{
    led1.on();
    led2.on();
    led3.on();
}
```
