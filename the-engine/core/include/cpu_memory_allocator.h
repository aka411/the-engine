#pragma once 

namespace TheEngine::Core
{

    class CPUMemoryAllocator : public ICPUMemoryAllocator
    {
        public:

         ~CPUMemoryAllocator() = default;
         
         virtual void* allocate(size_t size, size_t alignment = 16) override; // DEFAULT_ALIGNMENT is usually 8 or 16

         virtual void free(void* ptr) override;
    }

}