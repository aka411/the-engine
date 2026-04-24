#include "rendering-system/gpu-resource-system/gpu_resource_system.h"
#include <rendering-system/gpu-resource-system/gpu_mesh_system.h>
#include <rendering-system/gpu-resource-system/gpu_material_manager.h>
#include <rendering-system/gpu-resource-system/gpu_animation_manager.h>
//#include <rendering-system/gpu-resource-system/gpu_light_manager.h>

#include <rendering-system/rhi/i_render_device.h>




namespace TheEngine::RenderingSystem
{



        GPUResourceSystem::GPUResourceSystem(IRenderDevice& renderDevice) :

            m_renderDevice(renderDevice),
            m_bufferManager(renderDevice.getBufferManager()),
            m_transferManager(renderDevice.getTransferManager()),

            // Initializing the unique_ptrs
            m_gpuMeshSystem(std::make_unique<GPUMeshSystem>(m_bufferManager, m_transferManager)),
            m_gpuMaterialManager(std::make_unique<GPUMaterialManager>(m_bufferManager, m_transferManager, renderDevice.getTextureManager())),
           // m_gpuLightManager(std::make_unique<GPULightManager>()),
            m_gpuAnimationManager(std::make_unique<GPUAnimationManager>(m_bufferManager, m_transferManager))

           // m_textureManager(renderDevice.getTextureManager())
        {


        }

        GPUResourceSystem::~GPUResourceSystem()
        {


        }


        GPUMeshSystem& GPUResourceSystem::getGPUMeshSystem()
        {
            return *m_gpuMeshSystem.get();
        }

        GPUMaterialManager& GPUResourceSystem::getGPUMaterialManager()
        {
            return *m_gpuMaterialManager;
        }
        /*
        GPULightManager& GPUResourceSystem::getGPULightManager()
        {

            return *m_gpuLightManager;
        }
        */
        GPUAnimationManager& GPUResourceSystem::getGPUAnimationManager()
        {
            return *m_gpuAnimationManager;
        }





        ITextureManager& GPUResourceSystem::getTextureManager()
        {
            return m_renderDevice.getTextureManager();
        }



}


