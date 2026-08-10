#include <utils/event-bus/event_bus_connection.h>
#include <utils/event-bus/event_bus.h>

namespace TheEngine
{


	void EventBusConnection::cleanup()
	{
		if (m_eventBus != nullptr && m_connectionId != INVALID_CONNECTION_ID)
		{
			m_eventBus->unSubscribe(m_engineEventType, m_connectionId);

		}
	
		m_eventBus = nullptr;
		m_engineEventType = EngineEventType::NONE;
		m_connectionId = INVALID_CONNECTION_ID;
	}


	EventBusConnection::EventBusConnection(EventBus& eventBus, const EngineEventType eventType, const ConnectionId id) :
		m_eventBus{ &eventBus },
		m_engineEventType{ eventType },
		m_connectionId{ id }
	{

	}

	EventBusConnection::EventBusConnection(EventBusConnection&& other) noexcept
	{
	

		this->m_eventBus = other.m_eventBus;
		this->m_engineEventType = other.m_engineEventType;
		this->m_connectionId = other.m_connectionId;

		other.m_eventBus = nullptr;
		other.m_engineEventType = EngineEventType::NONE;
		other.m_connectionId = INVALID_CONNECTION_ID;

	}

	EventBusConnection& EventBusConnection::operator=(EventBusConnection&& other) noexcept
	{
		if (this == &other)
		{
			return *this;
		}

		cleanup();

		this->m_eventBus = other.m_eventBus;
		this->m_engineEventType = other.m_engineEventType;
		this->m_connectionId = other.m_connectionId;

		other.m_eventBus = nullptr;
		other.m_engineEventType = EngineEventType::NONE;
		other.m_connectionId = INVALID_CONNECTION_ID;

		return *this;
	}

	EventBusConnection::~EventBusConnection()
	{
		cleanup();
	}

}