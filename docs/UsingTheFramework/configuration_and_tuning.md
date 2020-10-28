# Configuring and Tuning embvm Projects

## Build System Options

The following meson project options can be set for this library when creating the build results directory with `meson`, or by using `meson configure`:

* `disable-builtins` will tell the compiler not to generate built-in function
* `disable-stack-protection` will tell the compiler not to insert stack protection calls
* `disable-rtti` will disable RTTI for C++ projects
* `disable-exceptions` will disable exceptions for C++ projects
* `enable-threading` can be used to control threaded targets and libc++ threading support
* `enable-pedantic`: Turn on `pedantic` warnings
* `enable-pedantic-error`: Turn on `pedantic` warnings and errors
* `hide-unimplemented-libc-apis`: Hides the header definitions for functions which are not actually implemented
* `enable-gnu-extensions` will enable GNU libc extensions that are implemented in this library

The following options can be used to configure the `libc++` used with this project:

* `libcxx-use-compiler-rt`
* `libcxx-use-llvm-libunwind`
* `libcxx-thread-library`
* `libcxx-has-external-thread-api`
* `libcxx-build-external-thread-api`
* `libcxx-enable-chrono`
* `libcxx-enable-filesystem`
* `libcxx-enable-stdinout`
* `libcxx-default-newdelete`
* `libcxx-silent-terminate`
* `libcxx-monotonic-clock`

Options can be specified using `-D` and the option name:

```
meson buildresults -Ddisable-builtins=false
```

The same style works with `meson configure`:

```
cd buildresults
meson configure -Ddisable-builtins=false
```

Need to talk about debug vs release (DEBUG=1, --debug, --optimization=X)

reconfiguring an existng project with make reconfig


## Selecting a Memory Allocator

## Customizing Log Macros



## Memory Tuning

For more information, see our [memory tuning guide](memory_tuning.md)
