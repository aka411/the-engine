#pragma once
#include "cpu_resource.h"
#include "../../core/include/types.h"
#include <vector>
#include <memory>
#include <stack>


namespace TheEngine::Resource
{
	


	class CPUResourceManager
	{

	private:
		std::vector<std::unique_ptr<CPUResource>> m_resources;
		std::stack<size_t> m_freeIndex;

	public:

		ResourceManager();

		TheEngine::Core::ResourceHandle addResource( std::unique_ptr<CPUResource>&& resource);

		template<typename DataType> 
		DataType* getResource(const TheEngine::Core::ResourceHandle handle)const;

		void removeResource(const TheEngine::Core::ResourceHandle handle);

		~ResourceManager();
	};


}