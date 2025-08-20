#pragma once 
#include <unordered_map>
#include <typeindex>
#include <functional>


namespace TheEngine::ECS
{
	//ComponentRegistry owner and creator of this

	struct ComponentTypeinfo
	{
		std::type_index typeId;

		size_t size = 0;
		size_t alignment = 0;

		std::function<void(void*)> constructor = nullptr; // Function to construct the component
		std::function<void(void*)> destructor = nullptr; // Function to destruct the component

		std::function<void(void*, const void*)> copyConstructor = nullptr; // Function to copy construct the component
		std::function<void(void*, void*)> moveConstructor = nullptr; // Function to move construct the component

	};


	class ComponentRegistry
	{

	private:



		// Internal maps to store registration data
		ComponentId m_nextComponentId = 0;

		std::unordered_map<std::type_index, ComponentId> m_typeToId;

		std::unordered_map<ComponentId, ComponentTypeinfo> m_IdToInfo;

	public:
		~ComponentRegistry() = default;
		ComponentRegistry() = default;

		template<typename DataType>
		ComponentId RegisterComponent(DataType component);

		ComponentTypeinfo* getComponentTypeinfo(ComponentId componentId) const;

		ComponentId getComponentId(std::type_index typeIndex) const;

		void removeComponent(ComponentId componentId);


	};





	template<typename DataType>
	ComponentId ComponentRegistry::RegisterComponent(DataType component)
	{
		std::type_index typeIdx = typeid(DataType);


		auto& it = m_typeToId.find(typeIdx);
		if (it != m_typeToId.end())
		{
			return it->second;
		}

		ComponentId componentId = m_nextComponentId++;
		m_typeToId[typeIdx] = componentId;


		ComponentTypeinfo typeInfo;
		typeInfo.typeId = typeIdx;
		typeInfo.size = sizeof(DataType);
		typeInfo.alignment = alignof(DataType);

		//constructors assignments

		//constructor
		typeInfo.constructor = [](void* destPtr)
			{
				new (destPtr) DataType();
			};
		//destructor
		typeinfo.destructor = [](void* objPtr)
			{
				static_cast<DataType*>(objPtr)->~DataType();
			};



		//copy constructor
		typeinfo.copyConstructor = [](void* destPtr, const void* srcPtr)
			{
				new (destPtr) DataType(*static_cast<const DataType*>(srcPtr));
			};

		//move constructor
		typeinfo.moveConstructor = [](void* destPtr, void* srcPtr)
			{
				new (destPtr) DataType(std::move(*static_cast<DataType*>(srcPtr)));
			};


		m_IdToInfo[componentId] = typeInfo;

		return componentId;

	}



}