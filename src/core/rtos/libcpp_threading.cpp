// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

// cppcheck-suppress preprocessorErrorDirective
#if __has_include(<__config>)
#include <__config>
#endif

#ifdef _LIBCPP_HAS_THREAD_API_EXTERNAL

#include <__external_threading>
#include <rtos/rtos.hpp>

_LIBCPP_BEGIN_NAMESPACE_STD

#pragma mark - Recursive Mutex Functions -

auto __libcpp_recursive_mutex_lock(__libcpp_recursive_mutex_t* _m) -> int
{
	(*_m)->lock();
	return 0;
}

auto __libcpp_recursive_mutex_trylock(__libcpp_recursive_mutex_t* _m) -> bool
{
	return (*_m)->trylock();
}

auto __libcpp_recursive_mutex_unlock(__libcpp_mutex_t* _m) -> int
{
	(*_m)->unlock();
	return 0;
}

auto __libcpp_mutex_lock(__libcpp_mutex_t* _m) -> int
{
	if((*_m) == nullptr)
	{
		__libcpp_mutex_init(_m);
	}

	(*_m)->lock();

	return 0;
}

auto __libcpp_mutex_trylock(__libcpp_mutex_t* _m) -> bool
{
	if((*_m) == nullptr)
	{
		__libcpp_mutex_init(_m);
	}

	return (*_m)->trylock();
}

auto __libcpp_mutex_unlock(__libcpp_mutex_t* _m) -> int
{
	if((*_m) == nullptr)
	{
		__libcpp_mutex_init(_m);
	}

	(*_m)->unlock();
	return 0;
}

#pragma mark - Condition Variable -

auto __libcpp_condvar_signal(__libcpp_condvar_t* _cv) -> int
{
	if((*_cv) == nullptr)
	{
		__libcpp_condvar_create(_cv);
	}

	(*_cv)->signal();
	return 0;
}

auto __libcpp_condvar_broadcast(__libcpp_condvar_t* _cv) -> int
{
	if((*_cv) == nullptr)
	{
		__libcpp_condvar_create(_cv);
	}

	(*_cv)->broadcast();
	return 0;
}

auto __libcpp_condvar_wait(__libcpp_condvar_t* _cv, __libcpp_mutex_t* _m) -> int
{
	if((*_cv) == nullptr)
	{
		__libcpp_condvar_create(_cv);
	}

	bool successful = (*_cv)->wait(*_m);

	return successful ? 0 : -1;
}

#pragma mark - Generic Threading Functions -

// Returns non-zero if the thread ids are equal, otherwise 0
auto __libcpp_thread_id_equal(__libcpp_thread_id t1, __libcpp_thread_id t2) -> bool
{
	return t1 == t2;
}

// Returns non-zero if t1 < t2, otherwise 0
auto __libcpp_thread_id_less(__libcpp_thread_id t1, __libcpp_thread_id t2) -> bool
{
	return t1 < t2;
}

void __libcpp_thread_yield()
{
	embvm::this_thread::yield();
}

void __libcpp_thread_sleep_for(const chrono::nanoseconds& _ns)
{
	embvm::this_thread::sleep_for(_ns);
}

auto __libcpp_thread_get_current_id() -> __libcpp_thread_id
{
	return embvm::this_thread::get_handle();
}

auto __libcpp_thread_get_id(const __libcpp_thread_t* _t) -> __libcpp_thread_id
{
	return (*_t)->native_handle();
}

auto __libcpp_thread_join(__libcpp_thread_t* _t) -> int
{
	(*_t)->join();
	return 0;
}

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP_HAS_THREAD_API_EXTERNAL
