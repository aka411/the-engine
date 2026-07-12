#pragma once
#include <cassert>
#include <vector>
#include <span>
#include <array>

#include <volk.h>
#include <utils/data-structures/my_in_place_vector.h>

namespace TheEngine::RenderingSystem::VulkanBackend
{

    class VulkanDescriptorSetManager;


    class VulkanDescriptorSetState
    {
        static constexpr uint32_t MAX_SETS = 4;
        static constexpr uint32_t MAX_BINDINGS_PER_SET = 2;
    private:

        struct PendingDescriptorWrite
        {
            bool dirty = false;
            VkDescriptorType type;

            union
            {
                VkDescriptorBufferInfo bufferInfo;
                VkDescriptorImageInfo imageInfo;

            }info;


            VkWriteDescriptorSet vkWriteDescriptorSet;

        };


        
       

        MyInPlaceVector<VkDescriptorSet, 4> m_VkDescriptorSets;

        std::array<std::vector<PendingDescriptorWrite>, 4> m_VkDescriptorSetWrites;



    public:


 
        VulkanDescriptorSetState();
        VulkanDescriptorSetState(VkCommandBuffer& vKCommandBuffer, VkPipelineLayout vkPipelineLayout, VkDescriptorSet globalBindlessTexturevkDescriptorSet, std::span<VkDescriptorSetLayout> vkDescriptorSetLayouts, VulkanDescriptorSetManager& vulkanDescriptorSetManager);

        ~VulkanDescriptorSetState();


        void bindBuffer(uint32_t set, uint32_t binding, VkDescriptorBufferInfo info, VkDescriptorType type);
        void flushUpdates(VkDevice device);



    };



}