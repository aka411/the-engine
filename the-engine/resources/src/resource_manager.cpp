#include "../include/resource_manager.h"



TheEngine::Resource::ResourceManager::ResourceManager()
{
	//not used
}


TheEngine::Resource::ResourceManager::~ResourceManager()
{
	// Cleanup code if needed
}

TheEngine::Core::ResourceHandle TheEngine::Resource::ResourceManager::addResource( std::unique_ptr<CPUResource>&& resource)
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
DataType* TheEngine::Resource::ResourceManager::getResource(const TheEngine::Core::ResourceHandle Handle)const
{

	if (static_cast<size_t>(handle) < m_resources.size()&& m_resources[handle])
	{
		CPUResource* baseResource = m_resources[handle].get();
		DataType* desiredResource = dynamic_cast<DataType*>(baseResource);
		return  desiredResource;
	}
	return nullptr;
}


void  TheEngine::Resource::ResourceManager::removeResource(const TheEngine::Core::ResourceHandle handle)
{
	if (handle >= 0 && static_cast<size_t>(handle) < m_resources.size() && m_resources[handle])
	{
		m_resources[handle].reset(); // Release the unique_ptr, destroying the resource
		m_freeIndex.push(handle);    // Add the freed index to the stack
	}
	// else: Handle invalid or already empty handle
}