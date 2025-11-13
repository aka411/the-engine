#pragma once
#include <shaderc/shaderc.hpp>
#include "volk.h"
#include <vulkan_device.h>

struct ShaderProgram
{
	VkShaderModule vertexShader = VK_NULL_HANDLE;
	VkShaderModule fragmentShader = VK_NULL_HANDLE;

};




enum class ShaderType
{
	VERTEX,
	FRAGMENT,
	COMPUTE
};

class ShaderManager
{

	//Owner of all shader modules
private:
	shaderc::Compiler m_SprivCompiler;
	shaderc::CompileOptions m_compileOptions;


	VulkanDevice& m_vulkanDevice;

	//need to store all shader modules created to destroy them later
	std::vector<VkShaderModule> m_shaderModules;

	ShaderProgram m_shaderProgramMVP; //hardcoded shader program for MVP





	const std::string m_vertexShaderCode = R"( 
#version 460
#extension GL_EXT_buffer_reference: require
#extension GL_EXT_shader_explicit_arithmetic_types_int64 : require // For 64-bit math


struct Vertex {
    vec4 position;
    vec3 color;
};


layout(buffer_reference,std430) readonly buffer VertexBuffer {
    Vertex data[];
};

layout(push_constant) uniform Constants {
    mat4 viewProjection;
    uint64_t vertexBufferAddress; // The Buffer Device Address of the SSBO         
} pc;

layout(location = 0) out vec3 fragColor;


void main() {

     VertexBuffer vertexBuffer = VertexBuffer(pc.vertexBufferAddress);
      

    vec4 position = vertexBuffer.data[gl_VertexIndex].position;
    vec3 color = vec3(vertexBuffer.data[gl_VertexIndex].color.xyz);


   


	gl_Position = vec4(position.xyz, 1.0);


fragColor = vec3(color);
}
)";

	const std::string m_fragmentShaderCode = R"(
#version 460
layout(location = 0) in vec3 fragColor;
layout(location = 0) out vec4 outColor;
void main() {
	outColor = vec4(fragColor, 1.0);
}
)";









	shaderc_shader_kind getShadercKind(ShaderType shaderType);	

public:
	ShaderManager(VulkanDevice& vulkanDevice);
	~ShaderManager();

	VkShaderModule CreateShaderModule(const std::string& codeString, ShaderType shadertype);

	//for MVP we will use a hardcoded shader program which is inside the ShaderManager
	ShaderProgram getShaderProgram();

};