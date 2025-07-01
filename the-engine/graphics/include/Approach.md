# Mesh and PSO(Shader interaction)

- The IMesh will have a vertex layout description that defines the vertex attributes.
- Like 


# Consider This 

- For grouping the layouts into one object

struct ShaderInputLayout
{
    std::vector<ShaderVertexAttribute> vertexAttributes;//VertexInputLayout
    std::vector<ShaderResourceDescription> resources;//DescriptorSetLayout
    // Hash or ID for quick comparison
};
