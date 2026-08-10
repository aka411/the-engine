#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <platform/data-structures/input_system_data_structures.h>
#include <limits>
#include <utils/event-bus/event_bus_connection.h>
#include <assert.h>

namespace TheEngine
{

	using EngineEventCallback = std::function<void(const EngineEvent&)>;

	class EventBus
	{
	private:

		size_t m_numberOfTimesUnsubscribeWasCalled{ 0 };

	private:

		std::unordered_map<EngineEventType, std::vector<EngineEventCallback>> m_subscribers;

	public:

		EventBus() = default;

		EventBus(const EventBus&) = delete;
		EventBus& operator=(const EventBus&) = delete;

		EventBus(EventBus&&) = delete;
		EventBus& operator=(EventBus&&) = delete;

		EventBusConnection subscribe(const EngineEventType eventType,const EngineEventCallback callback);
		void unSubscribe(const EngineEventType eventType,const ConnectionId id);

		void publish(const EngineEvent& event);
	};
}