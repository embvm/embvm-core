# Core Driver Build Definition

driver_files = files(
    'internal/gpio.cpp',
    'internal/i2c.cpp',
    'internal/led.cpp',
    'internal/basic_display.cpp',
    'internal/spi.cpp',
    'internal/system_clock.cpp',
    'internal/time_of_flight.cpp',
    'internal/timer.cpp'
)

# Because driver_base needs access to each platform's VirtualPlatform
# Definition, we separate that file out and trust the platform will resolve it at link time
driver_platform_files = files(
    'driver_base.cpp',
)

driver_test_files = files(
	'driver_test.cpp',
)
