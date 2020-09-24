# Customizing Logging Macros

The framework provides default log macro definitions in [log.hpp](../../../../src/subsystems/logging/log.hpp).

The log macros can be overridden by defining them in your platform's `platform_logger.hpp` file. Any macros defined from the following list will override the framework default definition:

* `logverbose`
* `logdebug`
* `loginfo`
* `logwarn`
* `logerror`
* `logcritical`

The only definition required in `platform_logger.hpp` is the macro itself:

```
#define logcritical(...) PlatformLogger::inst().log(logger::level::critical, __VA_ARGS__)
```

The framework will take care of compiling-out definitions according to the log level:

```
#if LOG_LEVEL >= LOG_LEVEL_CRITICAL
#ifndef logcritical
#define logcritical(...) PlatformLogger::inst().log(logger::level::critical, __VA_ARGS__)
#endif
#else
#define logcritical(...)
#endif
```
