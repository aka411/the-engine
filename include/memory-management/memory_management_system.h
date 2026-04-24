#pragma once
#include <memory>


namespace TheEngine::MemoryManagement
{

	//Owner of all allocators , gives out the allocators

	//TLSF allocator is the main allocator for the engine , it will be thread safe and give out for general purpose allocations,
	//other specialised allocators will not be thread safe and will be given out as a single object with link to the main allocator
	//for example , the stack allocator will be given out as a single object with link to the main allocator , and it will use the main allocator when it runs out of its pool


	//All allocators need to adher to give out std::max_align_t memory address and some allocator should have methods to give out custom aligned memory address upto 256 bytes alignement.

	class IAllocator
	{

	private:

		IAllocator& m_allocator;

	public:

		IAllocator();
			
	
		virtual ~IAllocator() = default;

		virtual void* allocate(const size_t size) = 0;
		virtual void deallocate(void* ptr) = 0;

		//virtual void* allocateAligned(const size_t size, const size_t alignment) = 0;

	};




	class MemoryManagementSystem
	{
	private:
		//
		std::unique_ptr<IAllocator> m_mainSystemAllocator;//TLSF allocator


	public:

		MemoryManagementSystem();


		//std::unique_ptr<IAllocator> getLinearAllocator();
		



	};






}