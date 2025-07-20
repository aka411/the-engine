#include "gpu_dynamic_buffer_allocator.h"

namespace TheEngine::Graphics

{
    GPUDynamicBufferAllocator::GPUDynamicBufferAllocator()
    {
    }
    //not compelete
    //might need redesign
    void* GPUDynamicBufferAllocator::allocate(size_t size, size_t alignment)
    {

        //Dont forget we are assuming required_size_for_this_frame is already padded for internal structs

        size_t alignedStartOffset = (m_current_offset_in_buffer + MIN_UBO_ALIGNMENT - 1) & ~(MIN_UBO_ALIGNMENT - 1);
        size_t actualRequiredSize= required_size_for_this_frame; 

        // Check if there's enough contiguous space from the aligned_start_offset to the end
        if ((aligned_start_offset + actual_required_size) > m_total_capacity)
        {
            // Not enough space at the end, so wrap around
            // Reset offset to the beginning of the buffer for the new allocation
            m_current_offset_in_buffer = 0;
            aligned_start_offset = 0; // Re-calculate aligned offset from the new start
        }
        return nullptr;
    }
}