// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#if __has_include(<__config>)
#include <__config>
#endif

#ifdef _LIBCPP_HAS_THREAD_API_EXTERNAL

#include <__external_threading>
#include <rtos/rtos.hpp>

_LIBCPP_BEGIN_NAMESPACE_STD

#pragma mark - Recursive Mutex Functions -

int __libcpp_recursive_mutex_lock(__libcpp_recursive_mutex_t* __m)
{
	(*__m)->lock();
	return 0;
}

bool __libcpp_recursive_mutex_trylock(__libcpp_recursive_mutex_t* __m)
{
	return (*__m)->trylock();
}

int __libcpp_recursive_mutex_unlock(__libcpp_mutex_t* __m)
{
	(*__m)->unlock();
	return 0;
}

int __libcpp_mutex_lock(__libcpp_mutex_t* __m)
{
	if((*__m) == nullptr)
	{
		__libcpp_mutex_init(__m);
	}

	(*__m)->lock();

	return 0;
}

bool __libcpp_mutex_trylock(__libcpp_mutex_t* __m)
{
	if((*__m) == nullptr)
	{
		__libcpp_mutex_init(__m);
	}

	return (*__m)->trylock();
}

int __libcpp_mutex_unlock(__libcpp_mutex_t* __m)
{
	if((*__m) == nullptr)
	{
		__libcpp_mutex_init(__m);
	}

	(*__m)->unlock();
	return 0;
}

#pragma mark - Condition Variable -

int __libcpp_condvar_signal(__libcpp_condvar_t* __cv)
{
	if((*__cv) == nullptr)
	{
		__libcpp_condvar_create(__cv);
	}

	(*__cv)->signal();
	return 0;
}

int __libcpp_condvar_broadcast(__libcpp_condvar_t* __cv)
{
	if((*__cv) == nullptr)
	{
		__libcpp_condvar_create(__cv);
	}

	(*__cv)->broadcast();
	return 0;
}

int __libcpp_condvar_wait(__libcpp_condvar_t* __cv, __libcpp_mutex_t* __m)
{
	if((*__cv) == nullptr)
	{
		__libcpp_condvar_create(__cv);
	}

	bool successful = (*__cv)->wait(*__m);

	return successful ? 0 : -1;
}

#pragma mark - Generic Threading Functions -

// Returns non-zero if the thread ids are equal, otherwise 0
bool __libcpp_thread_id_equal(__libcpp_thread_id t1, __libcpp_thread_id t2)
{
	return t1 == t2;
}

// Returns non-zero if t1 < t2, otherwise 0
bool __libcpp_thread_id_less(__libcpp_thread_id t1, __libcpp_thread_id t2)
{
	return t1 < t2;
}

void __libcpp_thread_yield()
{
	embvm::this_thread::yield();
}

void __libcpp_thread_sleep_for(const chrono::nanoseconds& __ns)
{
	embvm::this_thread::sleep_for(__ns);
}

__libcpp_thread_id __libcpp_thread_get_current_id()
{
	return embvm::this_thread::get_handle();
}

__libcpp_thread_id __libcpp_thread_get_id(const __libcpp_thread_t* __t)
{
	return (*__t)->native_handle();
}

int __libcpp_thread_join(__libcpp_thread_t* __t)
{
	(*__t)->join();
	return 0;
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_HAS_THREAD_API_EXTERNAL
