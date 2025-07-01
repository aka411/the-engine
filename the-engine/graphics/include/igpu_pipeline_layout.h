#pragma once

//cleaned
namespace TheEngine::Graphics
{
  
    class IGPUPipelineLayout
    {
    protected:
  
        IGPUPipelineLayout() = default;

    public:
        // Virtual destructor for proper polymorphic cleanup.
        virtual ~IGPUPipelineLayout() = default;


       // Implicit Indexing : The set index(or SetIndex) is determined by the position 
       // of an IGPUDescriptorSetLayout within this ordered list in the IGPUPipelineLayout.


        // Optionally, you might have methods to query its properties,
        // though typically its properties are fixed at creation.
        // virtual const std::vector<IGPUDescriptorSetLayout*>& GetSetLayouts() const = 0;
        // virtual const std::vector<PushConstantRange>& GetPushConstantRanges() const = 0;
    };
}