#pragma once

#include "../../core/include/types.h"
#include <vector>
#include <memory>
#include <stack>
#include "icpu_resource.h"


namespace TheEngine::Resource
{
	


	class CPUResourceManager
	{

	private:
		std::vector<std::unique_ptr<ICPUResource>> m_resources;
		std::stack<size_t> m_freeIndex;

	public:

		CPUResourceManager();

		TheEngine::Core::ResourceHandle addResource( std::unique_ptr<ICPUResource>&& resource);

		template<typename DataType> 
		DataType* getResource(const TheEngine::Core::ResourceHandle handle)const;

		void removeResource(const TheEngine::Core::ResourceHandle handle);


		//for testing

		size_t getResourceCount() const { return m_resources.size(); }
		bool isFreeIndexStackEmpty() const { return m_freeIndex.empty(); }
		int getTopFreeIndex() const { return m_freeIndex.empty() ? -1 : m_freeIndex.top(); }

		~CPUResourceManager();
	};


	template<typename DataType>
	DataType* CPUResourceManager::getResource(const TheEngine::Core::ResourceHandle handle)const
	{
		//consider if caller asks for a resource that is not of type DataType 
		if (static_cast<size_t>(handle) < m_resources.size() && m_resources[handle])
		{
			ICPUResource* baseResource = m_resources[handle].get();
			DataType* desiredResource = dynamic_cast<DataType*>(baseResource);
			return  desiredResource;
		}
		return nullptr;
	}


}