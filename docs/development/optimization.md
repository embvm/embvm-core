# Framework Application Optimization

**Table of Contents:**
1. [Configuration Options](#configuration-options)
2. [Reducing Binary Size](#reducing-binary-size)

## Configuration Options

## Reducing Binary Size

* Disable RTTI (disabled by default for framework applications)

### Debugging Options

If `printf` is not needed, you can eliminate it from your application for significant savings. Other tips to remove `printf`:

* Disable `always-enable-assert` (meson configuration option), because that will force `printf` usage with `assert()` calls.
* Eliminate all logging (`printf` usage)
* Eliminate all other `printf` calls
