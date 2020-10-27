## Unit Testing with Catch2

If you are using the [`embvm-project-skeleton`](https://github.com/embvm/embvm-project-skeleton) as a basis for your project, you will have a built-in catch2 testing target already configured for your project.

## Using the Built-in Catch2 Module

To register tests with the built-in module, simply add internal dependencies to the pre-defined `catch2_tests_dep` variable (created in the top-level `meson.build` file):

```
catch2_tests_dep += declare_dependency(
    sources: [
        boot_test_files,
        driver_test_files,
        hw_platform_test_files,
        platform_test_files,
        register_test_files,
    ]
)
```

## Running the Tests

If you use the built-in module, the `catch2` unit test program will automatically be registered with the `test` target, which will built and run all unit tests. You will get a summary of the test suite results when you invoke this target.

```
$ make test
ninja: Entering directory `buildresults'
[4/5] Running all tests.
1/1 embvm-core_catch2_test OK             0.74s


Ok:                 1
Expected Fail:      0
Fail:               0
Unexpected Pass:    0
Skipped:            0
Timeout:            0

Full log written to /Users/phillip/src/ea/framework/embvm-core/buildresults/meson-logs/testlog.txt
```

If your tests fail, you will see an indicate of which test suite failed, as well as information about the command invocation and paths to any error logs that were captured.

```
$ make test
ninja: Entering directory `buildresults'
[2/3] Running all tests.
1/1 embvm-core_catch2_test FAIL           0.60s (exit status 1)

The output from the failed tests:

1/1 embvm-core_catch2_test FAIL           0.60s (exit status 1)

--- command ---
21:28:25 /Users/phillip/src/ea/framework/embvm-core/buildresults/build/test/catch2/embvm-core_catch2_test -s -r junit -o /Users/phillip/src/ea/framework/embvm-core/buildresults/test/embvm-core_catch2_test.xml
-------


Summary of Failures:

1/1 embvm-core_catch2_test FAIL           0.60s (exit status 1)
1/1 embvm-core_catch2_test FAIL           0.60s (exit status 1)

Ok:                 0
Expected Fail:      0
Fail:               1
Unexpected Pass:    0
Skipped:            0
Timeout:            0

Full log written to /Users/phillip/src/ea/framework/embvm-core/buildresults/meson-logs/testlog.txt
FAILED: meson-test
/usr/local/Cellar/meson/HEAD-bf5bcad/libexec/bin/meson test --no-rebuild --print-errorlogs
ninja: build stopped: subcommand failed.
make: *** [test] Error 1
```

### Manually Running the Tests

The information provided above isn't very helpful, however. You will want to manually invoke the tests. If you're using the built-in module, you can find the path to the test program in the error output. But generally the test program will be found in:

```
BUILDRESULTS/build/test/catch2/PROJECT_NAME_catch2_test
```

> `BUILDRESUTLS` is your configured build output folder (defaults to `buildresults` if you're using the Makefile shim), and `PROJECT_NAME` is the name specified in your `project()` command invocation.

Running the program manually will give you detailed test failure output:

```
$ ./buildresults/build/test/catch2/embvm-core_catch2_test

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
embvm-core_catch2_test is a Catch v2.11.3 host application.
Run with -? for options

-------------------------------------------------------------------------------
Find a driver by name using hw platform
-------------------------------------------------------------------------------
../src/core/hw_platform/virtual_hw_platform_tests.cpp:14
...............................................................................

../src/core/hw_platform/virtual_hw_platform_tests.cpp:23: FAILED:
  CHECK( (1 + 2) == p.driverCount() )
with expansion:
  3 == 2

===============================================================================
test cases: 136 | 135 passed | 1 failed
assertions: 502 | 501 passed | 1 failed
```

### embvm-core Tests in Subproject Mode

Note that the `embvm-core` project tests are not going to be automatically compiled and executed if you're using it as a subproject. You can still manually build the test target and run the test, however.

## Writing Your Own Test Program

You are not required to use the Catch2 unit testing module. You can write your own test target, storing tests in the test folder or alongside your code.

To disable the built-in module, comment out this line in the top-level `meson.build` file:

```
subdir('build/test/catch2')
```
