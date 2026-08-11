#include <utils/event-bus/event_bus.h>


namespace TheEngine
{


	EventBusConnection EventBus::subscribe(const EngineEventType eventType, const EngineEventCallback callback)
	{
		EventBusConnection eventBusConnection(*this, eventType, m_subscribers[eventType].size());

		m_subscribers[eventType].push_back(callback);

		return eventBusConnection;
	}

	void EventBus::unSubscribe(const EngineEventType eventType, const ConnectionId id)
	{
		auto dummyCallBack = [](const EngineEvent& event) -> void
			{
				assert(false && "Dummy CallBack called");
			};

		auto it = m_subscribers.find(eventType);
		if (it != m_subscribers.end())
		{
			++m_numberOfTimesUnsubscribeWasCalled;
			assert(m_numberOfTimesUnsubscribeWasCalled <= 1000 && "Too many holes in vectors need to redesign EventBus");
			if (it->second.size() > id)
			{
				it->second[id] = dummyCallBack;
			}
		}
	}






	void EventBus::publish(const EngineEvent& event)
	{
		auto it = m_subscribers.find(event.engineEventType);
		if (it != m_subscribers.end())
		{
			for (const auto& subscriber : it->second)
			{
				subscriber(event);
			}

		}
	}

}