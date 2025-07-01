#pragma once
#include <vector>

struct VertexAttributeDescriptor
{
    TheEngine::Core::AssetID streamAssetID; // The AssetID for this vertex stream's data
    unsigned int bindingPoint;              // The binding point (slot) this stream connects to
    TheEngine::Core::MeshStreamType semanticType; // e.g., POSITION, NORMAL, TEXCOORD0
    Graphics::Format format;                // e.g., R32G32B32_SFLOAT for glm::vec3
    unsigned int offset;                    // Offset within the stream buffer (usually 0 if per-attribute buffer)
    unsigned int stride;                    // Stride of the data in this stream (e.g., sizeof(glm::vec3))
};


struct MeshComponent
{
    std::vector<VertexAttributeDescriptor> vertexStreams;

    TheEngine::Core::AssetID indexBufferID;


    // Default constructor
    MeshComponent() : indexBufferID(0) {} // Initialize AssetIDs to a default/invalid value

    // You might add helper methods to set/get streams if needed
    // For example, finding a stream by semantic type:
    const VertexAttributeDescriptor* getStream(TheEngine::Core::MeshStreamType type) const
    {
        for (const auto& stream : vertexStreams)
        {
            if (stream.semanticType == type)
            {
                return &stream;
            }
        }
        return nullptr; // Or throw an exception, depending on error handling
    }

};
