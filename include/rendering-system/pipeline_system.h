#pragma once
#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>
#include <memory>
#include "rhi/pipeline_builder.h"


namespace TheEngine::Platform
{
	class FileSystem;
	class Path;
}

namespace TheEngine::RenderingSystem
{


	class ShaderSystem;
	//class IPipelineBuilder;
	class IRenderDevice;

	class PipelineSystem
	{

	private:

		IRenderDevice& m_renderDevice;


		TheEngine::Platform::FileSystem& m_fileSystem;

		ShaderSystem& m_shaderSystem;

		PipelineStateConfig getPipelineStateConfigFromJson(const std::byte* byteData, const size_t size);

	public:

		PipelineSystem(IRenderDevice& renderDevice,TheEngine::Platform::FileSystem& fileSystem, ShaderSystem& shaderSystem);

		PipelineStateConfig loadPipelineConfigFromFile(const TheEngine::Platform::Path& pipelineStateConfigPath);

		VertexLayout loadVertexLayoutFromFile(const TheEngine::Platform::Path& vertexLayoutPath);

		RenderOutputConfiguration loadRenderOutputConfigurationFromFile(const TheEngine::Platform::Path& renderOutputConfigurationPath);


		PipelineHandle buildPipeline(const PipelineBuilder& pipelineBuilder);

	};


}