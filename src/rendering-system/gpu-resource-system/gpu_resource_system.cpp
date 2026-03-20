#include "rendering-system/gpu-resource-system/gpu_resource_system.h"
#include <rendering-system/gpu-resource-system/gpu_mesh_system.h>
#include <rendering-system/gpu-resource-system/gpu_material_manager.h>
#include <rendering-system/gpu-resource-system/gpu_animation_manager.h>
#include <rendering-system//gpu-resource-system/gpu_light_manager.h>
#include <rendering-system/low-level-gpu-systems/low_level_gpu_system.h>


namespace TheEngine::RenderingSystem
{



        GPUResourceSystem::GPUResourceSystem(
            LowLevelGPUSystem& lowLevelGPUSystem)

            :
            
            m_gpuBufferManager(lowLevelGPUSystem.getGPUBufferManager()),
            m_gpuBufferTransferManager(lowLevelGPUSystem.getGPUBufferTransferManager()),

            // Initializing the unique_ptrs
            m_gpuMeshSystem(std::make_unique<GPUMeshSystem>(m_gpuBufferManager, m_gpuBufferTransferManager)),
            m_gpuMaterialManager(std::make_unique<GPUMaterialManager>(m_gpuBufferManager, m_gpuBufferTransferManager)),
            m_gpuLightManager(std::make_unique<GPULightManager>(lowLevelGPUSystem)),
            m_gpuAnimationManager(std::make_unique<GPUAnimationManager>(m_gpuBufferManager, m_gpuBufferTransferManager))

        {


        }

        GPUResourceSystem::~GPUResourceSystem() = default;


        GPUMeshSystem& GPUResourceSystem::getGPUMeshSystem()
        {
            return *m_gpuMeshSystem.get();
        }

        GPUMaterialManager& GPUResourceSystem::getGPUMaterialManager()
        {
            return *m_gpuMaterialManager;
        }

        GPULightManager& GPUResourceSystem::getGPULightManager()
        {

            return *m_gpuLightManager;
        }

        GPUAnimationManager& GPUResourceSystem::getGPUAnimationManager()
        {
            return *m_gpuAnimationManager;
        }






}


