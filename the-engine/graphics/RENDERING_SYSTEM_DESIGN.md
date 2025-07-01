## Resources

### IGPUResource
 - The IGPUResource is the base interface for all GPU resources used in rendering.
	
	#### Specific GPU Resources
	- The below specific GPU resources are derived from the IGPUResource interface.
	- They define the interface for different types of GPU resources used in rendering.
          IGPUBuffer: Vertex buffers, index buffers, uniform buffers, shader storage buffers.
          IGPUTexture: 2D, 3D, Cube maps, texture arrays.
          IGPUShader: Vertex, fragment, compute, geometry shaders.
          IGPUPipelineState: Encapsulates blend state, depth state, rasterizer state, vertex layout, shader




	

### IGPURenderDevice
 - The IGPURenderDevice is the interface for the GPU rendering device. It provides methods for creating and managing GPU resources, such as textures, buffers, and shaders.
 - It also provides methods for submitting rendering commands to the GPU.
 - Methods for creating and deleting GPU resources, such as textures, buffers, and shaders.
	
	#### Concrete Implementations
	
	-D3D12GPURenderDevice (for DirectX 12)
    -VulkanGPURenderDevice (for Vulkan, no version unless specific need for distinct implementations)
    -OpenGLGPURenderDevice (for OpenGL, no version unless specific need for distinct implementations)
    -OpenGLESGPURenderDevice(for OpenGLES, no version unless specific need for distinct implementations)
	
	 ##### New requirements
	- OpenGLGPURenderDevice (minimum version 4.3 required) , requires glVertexAttribBinding.
	- OpenGLESGPURenderDevice (minimum version 3.1 required) requires glVertexAttribBinding.
	
	
	

	# Api Resource Handle 
	
	- Specific Apis like OpenGL,D3D12,Vulkan returns a api specific type like GLuint,uint64_t pointer,uint64_t 
	- respectively as Resource Handles.
	- In our design we are going to cast the Api specific Resource Handles to a common api agnostic type uint64_t 
	- which is large enough to accomodate all.
	- We will then cast it back to the Api specific type where ever necessary.




