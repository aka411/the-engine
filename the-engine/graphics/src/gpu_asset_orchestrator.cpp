#include "../include/gpu_asset_orchestrator.h"
#include "../../resources/include/cpu_resource.h"
#include "../../resources/include/cpu_mesh.h"
#include "../../resources/include/cpu_texture.h"
#include "../include/gpu_mesh.h"




TheEngine::Graphics::GPUAssetOrchestrator::GPUAssetOrchestrator(
	TheEngine::Resource::ResourceManager& cpuResourceManager,
	GPUResourceManager& gpuResourceManager,
	IGPURenderDevice& renderDevice)

	: m_cpuResourceManager(cpuResourceManager),
	  m_gpuResourceManager(gpuResourceManager),
	  m_renderDevice(renderDevice)
{
	// Constructor implementation
}





bool TheEngine::Graphics::GPUAssetOrchestrator::requestGPUUpload(const TheEngine::Core::ResourceHandle resourceHandle)
{




	TheEngine::Resource::CPUResource* cpuResource = m_cpuResourceManager.getResource<TheEngine::Resource::CPUResource>(resourceHandle);


	if (cpuResource == nullptr)
	{
		return false; // Resource not found
	}

switch(cpuResource->getType())
{
case TheEngine::Resource::CPUResourceType::MESH:
{
	auto cpuMesh = static_cast<TheEngine::Resource::CPUMesh*>( cpuResource);
	
	
		std::unique_ptr<TheEngine::Graphics::GPUMesh> gpuMesh = std::make_unique<TheEngine::Graphics::GPUMesh>();

		gpuMesh->setVertexLayout( cpuMesh->vertexInputLayout);

		for (int i = 0; i < cpuMesh->vertexBuffer.size(); ++i)
		{
			// Create a GPU buffer for each vertex buffer in the CPU mesh
			auto gpuBuffer = m_renderDevice.createBuffer(
				cpuMesh->vertexBuffer[i].data(),
				cpuMesh->vertexBuffer[i].size() * sizeof(float),
				TheEngine::Graphics::BufferUsage::VERTEX,
				TheEngine::Graphics::BufferCPUAccess::NONE //dont hardcode this, use a flag in the CPUResource
			);

			gpuMesh->addVertexBuffer(std::move(gpuBuffer));
		}

		m_gpuResourceManager.storeResource<TheEngine::Graphics::GPUMesh>(resourceHandle, std::move(gpuMesh));

		return true;


	}
	break;
}
case TheEngine::Resource::CPUResourceType::TEXTURE:
{
	return false; // Texture upload not implemented yet
	break;
}
default:
	return false; // Unsupported resource type}



}

