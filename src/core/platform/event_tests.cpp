// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "event_center.hpp"
#include "event_queue.hpp"
#include <atomic>
#include <catch2/catch.hpp>
#include <dispatch/dispatch.hpp>

using namespace embvm;

#pragma mark - Helper Functions -

static std::atomic<bool> callbackCalled;
static std::atomic<bool> callbackCalled2;
static std::atomic<bool> eventUpdate;

enum UnitTestSignals
{
	eventWithData = Event_EXTENSION_START
};

class TestEventWithData : public embvm::EventBase
{
  public:
	TestEventWithData() : embvm::EventBase(eventWithData) {}
	bool data = false;
};

static void subscribeTestCb(embvm::EventBase e)
{
	(void)e;
	callbackCalled = true;
}

static void passEventWithDataCb(embvm::EventBase e)
{
	TestEventWithData* event = static_cast<TestEventWithData*>(&e);

	eventUpdate = event->data;
}

#if 0
class TestActiveQueue final : public ActiveEventQueue<TestActiveQueue, TestEventWithData>
{
  public:
	size_t processed_cnt()
	{
		return processed_cnt_;
	}

	void process_event_(TestEventWithData e)
	{
		(void)e;
		processed_cnt_++;
	}

  protected:
	void process_(TestEventWithData e) final;

  private:
	size_t processed_cnt_ = 0;
};
#endif

#pragma mark - Test Cases -

TEST_CASE("Create empty static event", "[core/platform/event]")
{
	embvm::EventBase e;

	CHECK(embvm::Signal::Event_Invalid == e.sig);
	CHECK(false == e.safeToFree());
}

TEST_CASE("Create static event with signal", "[core/platform/event]")
{
	embvm::EventBase e(embvm::Signal::Event_ProcessorInitd);

	CHECK(embvm::Signal::Event_ProcessorInitd == e.sig);
	CHECK(false == e.safeToFree());
}

// TODO: disabled, allocation is currently broken.
// See issue #149
#if 0
TEST_CASE("Create empty dynamic event", "[core/platform/event]")
{
	auto e = embvm::EventBase::allocate();

	CHECK(nullptr != e);
	CHECK(embvm::Signal::Event_Invalid == e->sig);
	CHECK(true == e->safeToFree());
}

TEST_CASE("Create dynamic event with signal", "[core/platform/event]")
{
	auto e = embvm::EventBase::allocate(embvm::Signal::Event_ProcessorInitd);

	CHECK(nullptr != e);
	CHECK(embvm::Signal::Event_ProcessorInitd == e->sig);
	CHECK(true == e->safeToFree());
}
#endif

TEST_CASE("Create event manager", "[core/platform/event_manager]")
{
	embvm::DynamicEventCenter mgr;
}

TEST_CASE("Event manager creates static events", "[core/platform/event_manager]")
{
	embvm::DynamicEventCenter mgr;

	auto e_static = mgr.createSignal();
	auto e_static2 = mgr.createSignal(embvm::Signal::Event_ProcessorInitd);

	CHECK(embvm::Signal::Event_Invalid == e_static.sig);
	CHECK(embvm::Signal::Event_ProcessorInitd == e_static2.sig);
}

// TODO: disabled, allocation is currently broken.
// See issue #149
#if 0
TEST_CASE("Event manager creates events", "[core/platform/event_manager]")
{
	embvm::DynamicEventCenter mgr;

	auto e_dynamic = mgr.allocateSignal();

	CHECK(nullptr != e_dynamic);
	CHECK(embvm::Signal::Event_Invalid == e_dynamic->sig);
}
#endif

TEST_CASE("Event manager handles subscribe", "[core/platform/event_manager]")
{
	callbackCalled = false;
	embvm::DynamicEventCenter mgr;

	auto handle = mgr.subscribe(embvm::Signal::Event_ProcessorInitd, subscribeTestCb);

	CHECK(handle);
	CHECK(1 == mgr.numSubscribedEvents());
	CHECK(1 == mgr.numSubscribers(embvm::Signal::Event_ProcessorInitd));
}

TEST_CASE("Event manager handles unsubscribe", "[core/platform/event_manager]")
{
	callbackCalled = false;
	embvm::DynamicEventCenter mgr;

	auto handle = mgr.subscribe(embvm::Signal::Event_PlatformInitd, subscribeTestCb);
	auto handle2 = mgr.subscribe(embvm::Signal::Event_PlatformInitd, subscribeTestCb);

	CHECK(2 == mgr.numSubscribers(embvm::Signal::Event_PlatformInitd));

	mgr.unsubscribe(handle);

	CHECK(1 == mgr.numSubscribers(embvm::Signal::Event_PlatformInitd));

	mgr.unsubscribe(handle);

	CHECK(1 == mgr.numSubscribers(embvm::Signal::Event_PlatformInitd));

	handle2.reset();

	CHECK(0 == mgr.numSubscribers(embvm::Signal::Event_PlatformInitd));

	// TODO: why does this fail, but works later?
	// CHECK(0 ==  mgr.numSubscribedEvents());
}

TEST_CASE("Event manager handles subscriber + signal", "[core/platform/event_manager]")
{
	callbackCalled = false;
	embvm::DynamicEventCenter mgr;

	auto handle = mgr.subscribe(embvm::Signal::Event_ProcessorInitd, subscribeTestCb);
	mgr.signal(embvm::Signal::Event_ProcessorInitd);

	CHECK(callbackCalled);

	handle.reset();
}

TEST_CASE("Event manager allows multiple subscriptions", "[core/platform/event_manager]")
{
	callbackCalled = false;
	callbackCalled2 = false;
	embvm::DynamicEventCenter mgr;

	auto handle = mgr.subscribe(embvm::Signal::Event_ProcessorInitd, subscribeTestCb);
	auto handle2 = mgr.subscribe(embvm::Signal::Event_PlatformInitd, [](embvm::EventBase e) {
		(void)e;
		callbackCalled2 = true;
	});

	CHECK(2 == mgr.numSubscribedEvents());
	CHECK(1 == mgr.numSubscribers(embvm::Signal::Event_ProcessorInitd));
	CHECK(1 == mgr.numSubscribers(embvm::Signal::Event_PlatformInitd));

	mgr.signal(embvm::Signal::Event_ProcessorInitd);

	CHECK(callbackCalled);
	CHECK(false == callbackCalled2);

	mgr.signal(embvm::Signal::Event_PlatformInitd);

	CHECK(callbackCalled2);

	handle.reset();
	CHECK(1 == mgr.numSubscribedEvents());

	handle2.reset();
	CHECK(0 == mgr.numSubscribedEvents());
}

TEST_CASE("Event manager handles events with data", "[core/platform/event_manager]")
{
	eventUpdate = false;
	embvm::DynamicEventCenter mgr;
	TestEventWithData e;
	e.data = true;

	auto handle = mgr.subscribe(eventWithData, passEventWithDataCb);

	mgr.publish(e);

	CHECK(eventUpdate);

	handle.reset();
}

TEST_CASE("Event manager works with dynamic dispatch queue", "[core/platform/event_manager]")
{
	eventUpdate = false;
	embutil::DynamicDispatchQueue<> dispatch;
	embvm::DynamicEventCenter mgr(dispatch.getBoundDispatch());
	TestEventWithData e;
	e.data = true;

	auto handle = mgr.subscribe(eventWithData, passEventWithDataCb);

	mgr.publish(e);

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(eventUpdate);

	handle.reset();
}

TEST_CASE("Event manager works with static dispatch queue", "[core/platform/event_manager]")
{
	eventUpdate = false;
	embvm::StaticEventQueue<10> dispatch;
	embvm::DynamicEventCenter mgr(dispatch.getBoundDispatch());
	TestEventWithData e;
	e.data = true;

	auto handle = mgr.subscribe(eventWithData, passEventWithDataCb);

	mgr.publish(e);

	std::this_thread::sleep_for(std::chrono::milliseconds(1));

	CHECK(eventUpdate);

	handle.reset();
}

TEST_CASE("Create static Event manager: static map, static list", "[core/platform/event_manager]")
{
	eventUpdate = false;
	embvm::EventCenter<32, 1> mgr;
	TestEventWithData e;
	e.data = true;

	auto handle = mgr.subscribe(eventWithData, passEventWithDataCb);

	mgr.publish(e);

	CHECK(eventUpdate);

	handle.reset();
}

#pragma mark - Event Queue -

TEST_CASE("Create event queue", "[core/platform/event_queue]")
{
	EventQueue<TestEventWithData> event_queue;

	CHECK(true == event_queue.empty());
	CHECK(0 == event_queue.size());
}

TEST_CASE("Add/remove with event queue", "[core/platform/event_queue]")
{
	EventQueue<TestEventWithData> event_queue;

	TestEventWithData e;

	event_queue.push(e);

	CHECK(false == event_queue.empty());
	CHECK(1 == event_queue.size());

	event_queue.pop();

	CHECK(true == event_queue.empty());
}

#if 0
TEST_CASE("Create active event queue", "[core/platform/event_queue]")
{
	TestActiveQueue event_queue;

	CHECK(true == event_queue.empty());
	CHECK(0 == event_queue.size());
}

TEST_CASE("Add & process with active event queue", "[core/platform/event_queue]")
{
	TestActiveQueue event_queue;

	TestEventWithData e;

	event_queue.push(e);

	std::this_thread::sleep_for(std::chrono::milliseconds(2));

	CHECK(1 == event_queue.processed_cnt());
	CHECK(true == event_queue.empty());
}
#endif
