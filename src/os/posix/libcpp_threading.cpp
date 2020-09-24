#include <thread>

#ifdef _LIBCPP_HAS_THREAD_API_EXTERNAL

#include <__external_threading>
#include <errno.h>
#include <os.hpp>
#include <pthread.h>

using namespace os::posix;

_LIBCPP_BEGIN_NAMESPACE_STD

#pragma mark - Recursive Mutex Functions -

int __libcpp_recursive_mutex_init(__libcpp_recursive_mutex_t* __m)
{
	*__m = os::Factory::createMutex(embvm::mutex::type::recursive);
	return 0;
}

int __libcpp_recursive_mutex_destroy(__libcpp_recursive_mutex_t* __m)
{
	os::Factory::destroy(*__m);
	return 0;
}

#pragma mark - Mutex Functions -

int __libcpp_mutex_init(__libcpp_recursive_mutex_t* __m)
{
	*__m = os::Factory::createMutex();
	return 0;
}

int __libcpp_mutex_destroy(__libcpp_mutex_t* __m)
{
	// There is the possibility that we are destroying our mutex without creating it,
	// since we have to call lock, unlock, or try_lock to actually initialize a mutex.
	// This is because the mutex constructor is constexpr... We don't know why.
	if(*__m)
	{
		os::Factory::destroy(*__m);
	}

	return 0;
}

#pragma mark - Thread Functions -

int __libcpp_thread_create(__libcpp_thread_t* __t, void* (*__func)(void*), void* __arg)
{
	*__t =
		os::Factory::createThread("thread", reinterpret_cast<embvm::thread::func_t>(__func), __arg);
	return 0;
}

bool __libcpp_thread_isnull(const __libcpp_thread_t* __t)
{
	return *__t == 0;
}

int __libcpp_thread_destroy(__libcpp_thread_t* __t)
{
	if(__t && *__t)
	{
		os::Factory::destroy(const_cast<__libcpp_thread_t>(*__t));
		*__t = nullptr;
	}

	return 0;
}

int __libcpp_thread_detach(__libcpp_thread_t* __t)
{
	return reinterpret_cast<os::posix::Thread*>(*__t)->detach();
}

#pragma mark - Condition Variable -

int __libcpp_condvar_create(__libcpp_condvar_t* __cv)
{
	*__cv = os::Factory::createConditionVariable();
	return 0;
}

int __libcpp_condvar_timedwait(__libcpp_condvar_t* __cv, __libcpp_mutex_t* __m, timespec* __ts)
{
	if(!*__cv)
	{
		__libcpp_condvar_create(__cv);
	}

	auto success = reinterpret_cast<os::posix::ConditionVariable*>(*__cv)->timed_wait(*__m, *__ts);

	return success ? 0 : ETIMEDOUT;
}

int __libcpp_condvar_destroy(__libcpp_condvar_t* __cv)
{
	// There is the possibility that we are destroying our CV without creating it,
	// since we have to call a function to properly initialize it
	if(*__cv)
	{
		os::Factory::destroy(*__cv);
	}

	return 0;
}

#pragma mark - Execute Once -

// Execute once
int __libcpp_execute_once(__libcpp_exec_once_flag* flag, void (*init_routine)(void))
{
	return pthread_once(flag, init_routine);
}

#pragma mark - Thread Local Storage -

int __libcpp_tls_create(__libcpp_tls_key* __key, void (*__at_exit)(void*))
{
	return pthread_key_create(__key, __at_exit);
}

void* __libcpp_tls_get(__libcpp_tls_key __key)
{
	return pthread_getspecific(__key);
}

int __libcpp_tls_set(__libcpp_tls_key __key, void* __p)
{
	return pthread_setspecific(__key, __p);
}

_LIBCPP_END_NAMESPACE_STD

#endif
