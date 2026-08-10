#pragma once
#include <limits>
#include <platform/data-structures/input_system_data_structures.h>


namespace TheEngine
{


	using ConnectionId = size_t;
	constexpr ConnectionId INVALID_CONNECTION_ID = std::numeric_limits<ConnectionId>::max();

	class EventBus;

	class EventBusConnection
	{

	private:

		EventBus* m_eventBus{ nullptr };
		EngineEventType m_engineEventType{ EngineEventType::NONE };
		ConnectionId m_connectionId{ INVALID_CONNECTION_ID };

	private:

		void cleanup();

	public:

		EventBusConnection() = default;
		EventBusConnection(EventBus& eventBus, const EngineEventType eventType, const ConnectionId id);

		EventBusConnection(const EventBusConnection&) = delete;
		EventBusConnection& operator=(const EventBusConnection&) = delete;

		EventBusConnection(EventBusConnection&& other) noexcept;
		EventBusConnection& operator=(EventBusConnection&& other) noexcept;

		~EventBusConnection();

	};

}