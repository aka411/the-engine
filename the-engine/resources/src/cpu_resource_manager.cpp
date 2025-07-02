#include "../include/cpu_resource_manager.h"

namespace TheEngine::Resource
{




	TheEngine::Core::ResourceHandle CPUResourceManager::addResource(std::unique_ptr<CPUResource>&& resource)
	{
		TheEngine::Core::ResourceHandle handle;
		if (m_freeIndex.empty())
		{
			handle = m_resources.size();
			m_resources.push_back(std::move(resource));
		}
		else
		{
			handle = m_freeIndex.top();
			m_freeIndex.pop();
			m_resources[handle] = std::move(resource);
		}

		return handle;
	}
	template<typename DataType>
	DataType* CPUResourceManager::getResource(const TheEngine::Core::ResourceHandle Handle)const
	{

		if (static_cast<size_t>(handle) < m_resources.size() && m_resources[handle])
		{
			CPUResource* baseResource = m_resources[handle].get();
			DataType* desiredResource = dynamic_cast<DataType*>(baseResource);
			return  desiredResource;
		}
		return nullptr;
	}


	void  CPUResourceManager::removeResource(const TheEngine::Core::ResourceHandle handle)
	{
		if (handle >= 0 && static_cast<size_t>(handle) < m_resources.size() && m_resources[handle])
		{
			m_resources[handle].reset(); // Release the unique_ptr, destroying the resource
			m_freeIndex.push(handle);    // Add the freed index to the stack
		}
		// else: Handle invalid or already empty handle
	}
}