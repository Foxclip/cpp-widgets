#pragma once

#include <vector>
#include <functional>
#include <cassert>
#include <unordered_set>
#include "common/data_pointer_shared.h"

template<typename ...TArgs>
class Event;

template<typename ...TArgs>
class EventTarget {
public:
	EventTarget();
	virtual ptrdiff_t getId() const;
	virtual void operator()(TArgs... args) = 0;

private:
	ptrdiff_t id = -1;
	inline static size_t event_target_id = 0;

};

template<typename ...TArgs>
class EventHandlerFunc : public EventTarget<TArgs...> {
public:
	EventHandlerFunc(const std::function<void(TArgs...)>& func);
	void operator()(TArgs... args) override;

private:
	std::function<void(TArgs...)> func;
};

template<typename ...TArgs>
class EventHandlerEvent : public EventTarget<TArgs...> {
public:
	EventHandlerEvent(const Event<TArgs...>& event);
	ptrdiff_t getId() const override;
	void operator()(TArgs... args) override;

private:
	Event<TArgs...>* event;
};

template<typename ...TArgs>
class Event : public EventTarget<TArgs...> {
public:
	Event();
	Event(const Event<TArgs...>& event);
	Event<TArgs...>& operator=(const Event<TArgs...>& event);
	~Event();
	const std::vector<dp::DataPointerShared<EventTarget<TArgs...>>>& getTargets() const;
	static bool isLive(const Event<TArgs...>* event);
	void operator+=(const std::function<void(TArgs...)>& func);
	void operator+=(const Event<TArgs...>& event);
	void operator+=(const EventHandlerFunc<TArgs...>& handler);
	void operator-=(const EventTarget<TArgs...>& target);
	void operator()(TArgs... args);
	void clear();

private:
	std::vector<dp::DataPointerShared<EventTarget<TArgs...>>> targets;
	// All live Event objects of this type. EventHandlerEvent uses it to detect
	// that the event it forwards to has been destroyed, so that its raw event
	// pointer is never dereferenced.
	inline static std::unordered_set<const Event<TArgs...>*> live_events;

};

template<typename ...TArgs>
inline Event<TArgs...>::Event() : EventTarget<TArgs...>() {
	live_events.insert(this);
}

template<typename ...TArgs>
inline Event<TArgs...>::Event(const Event<TArgs...>& event) : EventTarget<TArgs...>(), targets(event.targets) {
	// The implicit copy constructor would skip this one, so this object has
	// to register itself explicitly.
	live_events.insert(this);
}

template<typename ...TArgs>
inline Event<TArgs...>& Event<TArgs...>::operator=(const Event<TArgs...>& event) {
	// No re-registration needed: this object was already registered when it
	// was constructed.
	if (this != &event) {
		this->targets = event.targets;
	}
	return *this;
}

template<typename ...TArgs>
inline Event<TArgs...>::~Event() {
	live_events.erase(this);
}

template<typename ...TArgs>
inline const std::vector<dp::DataPointerShared<EventTarget<TArgs...>>>& Event<TArgs...>::getTargets() const {
	return targets;
}

template<typename ...TArgs>
inline bool Event<TArgs...>::isLive(const Event<TArgs...>* event) {
	return live_events.find(event) != live_events.end();
}

template<typename ...TArgs>
inline void Event<TArgs...>::operator+=(const std::function<void(TArgs...)>& func) {
	dp::DataPointerShared<EventTarget<TArgs...>> uptr = dp::make_shared_data_pointer<EventHandlerFunc<TArgs...>>("EventHandler func", func);
	targets.push_back(std::move(uptr));
}

template<typename ...TArgs>
inline void Event<TArgs...>::operator+=(const Event<TArgs...>& event) {
	dp::DataPointerShared<EventTarget<TArgs...>> uptr = dp::make_shared_data_pointer<EventHandlerEvent<TArgs...>>("EventHandler event", event);
	targets.push_back(std::move(uptr));
}

template<typename ...TArgs>
inline void Event<TArgs...>::operator+=(const EventHandlerFunc<TArgs...>& handler) {
	dp::DataPointerShared<EventTarget<TArgs...>> uptr = dp::make_shared_data_pointer<EventHandlerFunc<TArgs...>>("EventHandler EventHandlerFunc", handler);
	targets.push_back(std::move(uptr));
}

template<typename ...TArgs>
inline void Event<TArgs...>::operator-=(const EventTarget<TArgs...>& target) {
	auto pred = [&](const dp::DataPointerShared<EventTarget<TArgs...>>& element) {
		return element->getId() == target.getId();
	};
	std::erase_if(targets, pred);
}

template<typename ...TArgs>
inline void Event<TArgs...>::operator()(TArgs ...args) {
	for (dp::DataPointerShared<EventTarget<TArgs...>>& target : targets) {
		EventTarget<TArgs...>* ptr = target.get();
		(*ptr)(args...);
	}
}

template<typename ...TArgs>
inline void Event<TArgs...>::clear() {
	targets = std::vector<dp::DataPointerShared<EventTarget<TArgs...>>>();
}

template<typename ...TArgs>
inline EventTarget<TArgs...>::EventTarget() {
	this->id = event_target_id++;
}

template<typename ...TArgs>
inline ptrdiff_t EventTarget<TArgs...>::getId() const {
	return id;
}

template<typename ...TArgs>
inline EventHandlerFunc<TArgs...>::EventHandlerFunc(const std::function<void(TArgs...)>& func) : EventTarget<TArgs...>() {
	this->func = func;
}

template<typename ...TArgs>
inline void EventHandlerFunc<TArgs...>::operator()(TArgs...args) {
	func(args...);
}

template<typename ...TArgs>
inline EventHandlerEvent<TArgs...>::EventHandlerEvent(const Event<TArgs...>& event) : EventTarget<TArgs...>() {
	this->event = const_cast<Event<TArgs...>*>(&event);
}

template<typename ...TArgs>
inline ptrdiff_t EventHandlerEvent<TArgs...>::getId() const {
	// Never dereference the event pointer if the event has been destroyed;
	// fall back to this handler's own id.
	if (!Event<TArgs...>::isLive(this->event)) {
		return EventTarget<TArgs...>::getId();
	}
	return this->event->getId();
}

template<typename ...TArgs>
inline void EventHandlerEvent<TArgs...>::operator()(TArgs...args) {
	// The event this handler forwards to may have been destroyed after the
	// handler was created. Checking the live-events registry is safe, while
	// dereferencing the (possibly dangling) event pointer directly is not.
	if (Event<TArgs...>::isLive(this->event)) {
		(*this->event)(args...);
	}
}
