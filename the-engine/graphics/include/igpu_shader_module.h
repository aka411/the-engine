#pragma once
#include <cstdint>
#include <string>

//cleaned
namespace TheEngine::Graphics
{
    enum class ShaderStage : uint32_t
    {
        VERTEX,
        TESSELLATION_CONTROL,
        TESSELLATION_EVALUATION,
        GEOMETRY,
        FRAGMENT,
        COMPUTE

    };

    struct ShaderModuleCreateInfo
    {
        ShaderStage stage;
        std::vector<uint8_t> spirvCode; // Or byte code, or source code string
        std::string entryPoint;         // Main function name (e.g., "main")
     
    };



    //The OpenGLGraphicsPipelineState will then perform the glCreateProgram(), 
    // glAttachShader() for each stage, and glLinkProgram() calls.


    class IGPUShaderModule : public IGPUResource
    {
    protected:

        IGPUShaderModule() = default;//correct

    public:
        virtual ~IGPUShaderModule() = default;

        virtual ShaderStage GetStage() const = 0;
        virtual const std::string& GetEntryPoint() const = 0;

    };
}