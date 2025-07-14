#pragma once

#include "../../core/include/types.h"
#include <vector>
#include <memory>
#include <stack>
#include "icpu_resource_manager.h"
#include "icpu_resource.h"


namespace TheEngine::Resource
{
	


	class CPUResourceManager : ICPUResourceManager
	{

	private:
		std::vector<std::unique_ptr<ICPUResource>> m_resources;
		std::stack<size_t> m_freeIndex;

	public:

		CPUResourceManager();

		TheEngine::Core::ResourceHandle addResource( std::unique_ptr<ICPUResource>&& resource);

		virtual ICPUResource* getCPUResource(const TheEngine::Core::ResourceHandle handle) const override;

		void removeResource(const TheEngine::Core::ResourceHandle handle);


		//for testing

		size_t getResourceCount() const { return m_resources.size(); }
		bool isFreeIndexStackEmpty() const { return m_freeIndex.empty(); }
		int getTopFreeIndex() const { return m_freeIndex.empty() ? -1 : m_freeIndex.top(); }

		~CPUResourceManager();
	};



}