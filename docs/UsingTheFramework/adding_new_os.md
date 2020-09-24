# Adding a New Driver Type

* `meson.build`
* mutex
* semaphore
* message queue
* event queue
* thread
* CV


Talk about the helper script once it is created

## libcpp Support

STL threading constructs can be supported by supplying the functions required by the `__external_threading` interface header in libcpp. 

Common functions which can use the Virtual RTOS interfaces are implemented in the `core/rtos/libcpp_threading.cpp` file. Functions which tend to be OS-specific are in each OS's respective `libcpp_threading.cpp` file. These OS-specific implementations are:

* `__libcpp_recursive_mutex_init`
* `__libcpp_recursive_mutex_destroy`
* `__libcpp_mutex_init`
* `__libcpp_mutex_destroy`
* `__libcpp_thread_create`
* `__libcpp_thread_isnull` (used as a destroy function)
* `__libcpp_condvar_create`
* `__libcpp_condvar_timedwait`
* `__libcpp_condvar_destroy`
* `__libcpp_tls_create`
* `__libcpp_tls_get`
* `__libcpp_execute_once`

Functions which are not supported on an OS port should return `-1` to indicate failure. The C++ library will abort on failing interface calls.

### Understanding libcpp's use of Thread Local Storage

libcpp wraps std::thread objects with a proxy function. This proxy function registers a private data pointer using the `__libcpp_tls_*` APIs. This can be a bit problematic for our threading implementations.

Some implementations, like pthread, supports Thread Local Storage (TLS) that matches the standard's interface expectations. This is not accidental - the standard was designed around POSIX.

<center>
![The libcpp std::thread construction flow is complex and involves multiple objects.](images/libcpp_Thread_Construction.png "libcpp `std::thread` Construction Flow")
</center>

When `__libcpp_tls_create`, a function is registered for the specific key value. This function is to be called when *each thread using that key* exits. libcpp uses this behavior to call a destructor to released allocated thread data. We have to solve this problem to use `std::thread`, or we will leak data every time a thread is deleted. (Perhaps only a small problem for your platform, but one the framework needs to solve!)

Once the key and destructor function have been initialized, the pointer to that thread's specific data is stored in the thread local storage. We only store one `void*` value.

Your OS port must support the following:

1. Register a destructor function to be called when threads terminate
2. Store a pointer to libcpp's allocated data in some form of thread-local storage
3. When a thread terminates, call the destructor function (if registered) and pass in the thread local data (if present).

For an example implementation "workaround", look at the [FreeRTOS OS implementation](../../src/os/freertos):

* [thread.hpp](../../src/os/freertos/thread.hpp)
* [thread.cpp](../../src/os/freertos/thread.cpp)
* [libcpp_threading.cpp](../../src/os/freertos)

There is a global destructor registration function and storage variable. The OS port only supports one function at this time, since we are only anticipating its use by libcpp:

```
static volatile thread_exit_t exit_func_ = nullptr;

// TODO: do we want to static assert and assume configUSE_TIME_SLICING?

void os::freertos::register_threadexit_func(thread_exit_t atexit)
{
    // We only support one func at this time, and it's for libcpp
    assert(exit_func_ == nullptr); 
    exit_func_ = atexit;
}
```

Terminate reads from the local storage data before deleting the thread. Once the thread is deleted, we call the exit function and pass in the TLS pointer (if both are set):

```
void Thread::terminate()
{
    /// Grab our TLS handle before we destroy the thread
    auto pdata = reinterpret_cast<void*>(pvTaskGetThreadLocalStoragePointer(NULL, 0));

    if(handle_)
    {
        vTaskDelete(reinterpret_cast<TaskHandle_t>(handle_));

#if configSUPPORT_STATIC_ALLOCATION
        if(static_)
        {
            static_thread_pool_.release(reinterpret_cast<StaticTask_t*>(handle_));
        }
#endif

        // This block is used for libcpp support. If there is TLS data and a registered exit func,
        // we need to destroy libcpp's data accordingly.
        if(exit_func_ && pdata)
        {
            exit_func_(pdata);
        }

        handle_ = 0;
    }
}
```

We also ensure that FreeRTOS has a TLS pointer configuration at compile-time:

```
static_assert(configNUM_THREAD_LOCAL_STORAGE_POINTERS > 0,
              "libcpp requires a FreeRTOS config that supports thread local storage. "
              "Set the configNUM_THREAD_LOCAL_STORAGE_POINTERS parameter.");
```

We create the key by simply registering the exit function:

```
int std::__libcpp_tls_create(std::__libcpp_tls_key* __key, void (_LIBCPP_TLS_DESTRUCTOR_CC*__at_exit)(void*))
{
    os::freertos::register_threadexit_func(__at_exit);
    return 0;
}
```

`__libcpp_tls_get` reads from the FreeRTOS TLS storage, ignoring the key and always using index 0:

```
void* std::__libcpp_tls_get(std::__libcpp_tls_key __key)
{
    return reinterpret_cast<void*>(pvTaskGetThreadLocalStoragePointer(NULL, 0));
}
```

`__libcpp_tls_set` ignores the key and stores the pointer in index 0:

```
int std::__libcpp_tls_set(std::__libcpp_tls_key __key, void* __p)
{
    (void) __key;
    vTaskSetThreadLocalStoragePointer(NULL, 0, __p);
    return 0;
}
```

Workarounds like this are acceptable. If other TLS support is required, they must be more thoughtfully designed.
