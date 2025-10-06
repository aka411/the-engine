#pragma once
#include <functional>
#include <typeindex>

namespace TheEngine::ECS
{
	using ComponentId = unsigned int;



	struct ComponentTypeInfo
	{
		std::type_index typeId;

		size_t size = 0;
		size_t alignment = 0;

		std::function<void(void*)> constructor = nullptr; // Function to construct the component
		std::function<void(void*)> destructor = nullptr; // Function to destruct the component

		std::function<void(void*, const void*)> copyConstructor = nullptr; // Function to copy construct the component
		std::function<void(void*, void*)> moveConstructor = nullptr; // Function to move construct the component

	};



	class IComponentRegistry
	{

	private:

	public:

		virtual ComponentTypeInfo* getComponentTypeinfo(ComponentId componentId) const = 0;


	};

}