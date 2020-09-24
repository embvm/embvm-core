# CRC-R: Virtual RTOS

Provides an abstraction layer for RTOS/OS concepts.

**Status:** Completed

**Table of Contents:**

1. [Responsibilities](#responsibilities)
2. [Requirements](#requirements)
3. [Collaborators](#collaborators)
4. [Rationale](#rationale)
5. [Source Links](#source-links)
6. [Related Documents](#related-documents)
7. [Notes](#notes)

## Responsibilities

* RTOS Initialization
* Management of Threads
* Management of Mutexes
* Management of Semaphores
* Management of Events
* Management of Condition Variables

## Requirements

* Generalized interface should support multiple RTOSes, including:
    * ThreadX
    * FreeRTOS
* POSIX support shall be added (e.g., via `pthread`) to allow host-machine simulation with the framework
* Developers must be able to specify thread priorities

## Collaborators

* A [Virtual Platform](virtual_platform.md) may or may not utilize a Virtual RTOS
* An RTOS often depends upon a [Virtual Processor](virtual_processor.md) or [Virtual Processor Architecture](virtual_processor_architecture.md)

## Rationale

Embedded systems developers often directly call RTOS functions within their programs. If the team is required to move to a new processor, the RTOS may need to change, resulting in a large porting effort.

Instead, we can provide a generalized virtual RTOS model for developers to utilize. The RTOS is decided upon in the [Virtual Platform](virtual_platform.md). An `OSFactory` can then be used by the Platform and Software layers to ensure that they remain decoupled from the underlying implementation.

Additionally, a Virtual RTOS allows for a single libcpp threading implementation to be designed, rather than one for each RTOS.

* The Virtual RTOS will be implemented using the [Bridge Pattern](../../../patterns/bridge.md)
* The Virtual RTOS will likely utilize an [Abstract Factory Pattern](../../../patterns/abstract_factory.md) for creation

Bridge pattern for OS abstraction:

```
os::CreateThread()
{
	impl_->createThread();
}
```

## Source Links

* [Virtual RTOS](../../../../src/core/rtos/)
    * [Factory APIs](../../../../src/core/rtos/rtos.hpp)
    * [Condition Variable APIs](../../../../src/core/rtos/condition_variable.hpp)
    * [Event Flag APIs](../../../../src/core/rtos/event_flag.hpp)
    * [Message Queue APIs](../../../../src/core/rtos/msg_queue.hpp)
    * [Mutex APIs](../../../../src/core/rtos/mutex.hpp)
    * [Semaphore APIs](../../../../src/core/rtos/semaphore.hpp)
    * [Thread APIs](../../../../src/core/rtos/thread.hpp)
    * [Tests](../../../../src/core/rtos/rtos_tests.cpp)
* [Unit Test OS](../../../../src/os/unit_test)
* [POSIX OS](../../../../src/os/posix)

## Notes

Since the Virtual RTOS utilizes the [Bridge Pattern](../../../patterns/bridge.md), two sets of underlying components are required:

1. A Virtual RTOS Abstraction (generalized interface)
2. A Virtual RTOS Implementation (per RTOS)

### API References

ThreadX References

* ThreadX API Reference Manual (PDF)

FreeRTOS References

* [FreeRTOS API](https://www.freertos.org/a00106.html)
* [Task Creation](https://www.freertos.org/a00019.html)
* [Semaphores](https://www.freertos.org/a00113.html)
* [Event Groups](https://www.freertos.org/event-groups-API.html)
* [Message Buffers](https://www.freertos.org/RTOS-message-buffer-API.html)

`pthread` References

* Thread Management
    * We initialize attributes, then create the thread with the attributes
    * [POSIX Thread Management](http://www.codeconfidence.com/doc/ecos/current/ref/posix-thread-management.html)
    * [Pthreads notes](http://www.cs.fsu.edu/~baker/realtime/restricted/notes/pthreads.html)
* Mutex
    * [`pthread_mutex_lock`])http://pubs.opengroup.org/onlinepubs/009695399/functions/pthread_mutex_lock.html)
    * [`pthread_mutexattr_gettype`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_gettype.html#)
    * [`pthread_mutexattr_getprotocol`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_getprotocol.html#)
    * [`pthread_mutexattr_getprioceiling`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_getprioceiling.html#)
    * [`pthread_mutex_init`](https://linux.die.net/man/3/pthread_mutex_init)
* Message Queue
    * [Message Queue Overview](https://users.pja.edu.pl/~jms/qnx/help/watcom/clibref/mq_overview.html)

#### Suspend/Resume for PThreads

`pthread` suspend/resume support must be implemented manually. At this time we are not supporting suspend/resume operations in the Virtual RTOS interface.

Here is some example code gleaned from the Internet:

```
void suspendMe()
{ // tell the thread to suspend
    pthread_mutex_lock(&m_SuspendMutex);
    m_SuspendFlag = 1;
    pthread_mutex_unlock(&m_SuspendMutex);
}
void resumeMe()
{ // tell the thread to resume
    pthread_mutex_lock(&m_SuspendMutex);
    m_SuspendFlag = 0;
    phtread_cond_broadcast(&m_ResumeCond);
    pthread_mutex_unlock(&m_SuspendMutex);
}
void checkSuspend()
{ // if suspended, suspend until resumed
    pthread_mutex_lock(&m_SuspendMutex);
    while (m_SuspendFlag != 0) pthread_cond_wait(&m_ResumeCond, &m_SuspendMutex);
    pthread_mutex_unlock(&m_SuspendMutex);
}
```

The thread should call `checkSuspend` at safe points where it can be suspended. Other threads can call `suspendMe` and `resumeMe` to suspend/resume the thread.

Notice that now the mutex protects the `m_SuspendFlag variable`. This ensures that the thread is told to suspend, told to resume, and checks whether it should suspend or stay suspended under protection. This approach makes the code thread-safe.
