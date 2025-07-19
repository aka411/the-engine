#pragma once 

namespace TheEngine::Core
{

    class ICPUMemoryAllocator
    {

        public:

        ~ICPUMemoryAllocator() = default;
         
         virtual void* allocate(size_t size, size_t alignment = 16) = 0; // DEFAULT_ALIGNMENT is usually 8 or 16

         virtual void free(void* ptr) = 0;


    };

}