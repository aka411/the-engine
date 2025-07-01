#pragma once

//consider another design , no need to clean for now
class RenderGraph
{
public:
    // Represents a logical render pass (a node in the graph)
    struct PassDescriptor
    {
        std::string name;
        std::function<void(ICommandRecorder*, /* other pass-specific data */)> executeFn;
        // Declare resource inputs/outputs (e.g., GBuffer textures, shadow maps)
        std::vector<ResourceHandle> readResources;
        std::vector<ResourceHandle> writeResources;
        // Other flags (e.g., is this a compute pass?)
    };

    void addPass(const PassDescriptor& pass);

    // This is where the magic happens:
    // - Traverses the graph to determine execution order
    // - Creates API-specific render passes and framebuffers
    // - Inserts necessary barriers/transitions
    void compileAndExecute(ICommandRecorder* recorder);

private:
    std::vector<PassDescriptor> m_passes;
    // Internal data structures for graph traversal, resource tracking, etc.
};