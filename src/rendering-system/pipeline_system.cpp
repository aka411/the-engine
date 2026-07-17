#include <rendering-system/pipeline_system.h>
#include <platform/file_system.h>
#include <nlohmann/json.hpp>
#include <rendering-system/pipeline_json_loader.h>
#include <rendering-system/rhi/i_render_device.h>
#include <rendering-system/rhi/i_pipeline_manager.h>
namespace TheEngine::RenderingSystem
{



	PipelineStateConfig PipelineSystem::getPipelineStateConfigFromJson(const std::byte* byteData, const size_t size)
	{


		auto json = nlohmann::json::parse(
			reinterpret_cast<const uint8_t*>(byteData),
			reinterpret_cast<const uint8_t*>(byteData + size));



		PipelineStateConfig pipelineStateConfig;

		from_json(json, pipelineStateConfig);
	
	


		return pipelineStateConfig;
	}








	PipelineSystem::PipelineSystem(IRenderDevice& renderDevice, TheEngine::Platform::FileSystem& fileSystem, ShaderSystem& shaderSystem):
	    m_renderDevice(renderDevice),
	    m_fileSystem(fileSystem),
		m_shaderSystem(shaderSystem)
	{

	}

	PipelineStateConfig PipelineSystem::loadPipelineConfigFromFile(const TheEngine::Platform::Path& configPath)
	{
		auto file = m_fileSystem.open(configPath);

		assert(file.isValid() && "PipelineConfig file is not valid");

		return getPipelineStateConfigFromJson(file.begin(), file.size());
	}


	VertexLayout PipelineSystem::loadVertexLayoutFromFile(const TheEngine::Platform::Path& vertexLayoutPath)
	{

		auto file = m_fileSystem.open(vertexLayoutPath);

		assert(file.isValid() && "VertexLayout file not valid");

		auto json = nlohmann::json::parse(
			reinterpret_cast<const uint8_t*>(file.begin()),
			reinterpret_cast<const uint8_t*>(file.end()));


		VertexLayout vertexLayout;

		from_json(json, vertexLayout);


		return vertexLayout;
	}





	RenderOutputConfiguration PipelineSystem::loadRenderOutputConfigurationFromFile(const TheEngine::Platform::Path& renderOutputConfigurationPath)
	{


		auto file = m_fileSystem.open(renderOutputConfigurationPath);

		assert(file.isValid() && "RenderOutputConfiguration file is not valid");

		auto json = nlohmann::json::parse(
			reinterpret_cast<const uint8_t*>(file.begin()),
			reinterpret_cast<const uint8_t*>(file.end()));


		RenderOutputConfiguration renderOutputConfiguration;

		from_json(json, renderOutputConfiguration);


		return renderOutputConfiguration;


	}

	PipelineHandle PipelineSystem::buildPipeline(const PipelineBuilder& pipelineBuilder)
	{
		return m_renderDevice.getPipelineManager().createPipeline(pipelineBuilder.getPipelineName(), pipelineBuilder.getPipelineStateCreateInfo());
	}

}