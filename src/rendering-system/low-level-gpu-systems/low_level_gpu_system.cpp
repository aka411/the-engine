#include "rendering-system/low-level-gpu-systems/low_level_gpu_system.h"

#include "rendering-system/low-level-gpu-systems/gpu_buffer_manager.h"
#include "rendering-system/low-level-gpu-systems/gpu_sampler_manager.h"
#include "rendering-system/low-level-gpu-systems/gpu_shader_manager.h"
#include "rendering-system/low-level-gpu-systems/gpu_texture_manager.h"
#include <rendering-system/low-level-gpu-systems/gpu_pipeline_manager.h>

#include "rendering-system/low-level-gpu-systems/memory-management/gpu_buffer_transfer_manager.h"


namespace TheEngine::RenderingSystem
{
    LowLevelGPUSystem::LowLevelGPUSystem()
        :

        m_gpuBufferManager(std::make_unique<GPUBufferManager>()),
        m_gpuBufferTransferManager(std::make_unique<GPUBufferTransferManager>(*m_gpuBufferManager.get())),

        m_gpuSamplerManager(std::make_unique<GPUSamplerManager>()),
        m_gpuTextureManager(std::make_unique<GPUTextureManager>(*m_gpuSamplerManager.get())),
        m_gpuShaderManager(std::make_unique<GPUShaderManager>()),
        m_gpuPipelineManager(std::make_unique<GPUPipelineManager>())
   


    {


    }

  
    LowLevelGPUSystem::~LowLevelGPUSystem() = default;

    GPUBufferTransferManager& LowLevelGPUSystem::getGPUBufferTransferManager() { return *m_gpuBufferTransferManager; }
    GPUBufferManager& LowLevelGPUSystem::getGPUBufferManager() { return *m_gpuBufferManager; }
    GPUTextureManager& LowLevelGPUSystem::getGPUTextureManager() { return *m_gpuTextureManager; }
    GPUShaderManager& LowLevelGPUSystem::getGPUShaderManager() { return *m_gpuShaderManager; }
    GPUSamplerManager& LowLevelGPUSystem::getGPUSamplerManager() { return *m_gpuSamplerManager; }

    GPUPipelineManager& LowLevelGPUSystem::getGPUPipelineManager() { return *m_gpuPipelineManager; }

}