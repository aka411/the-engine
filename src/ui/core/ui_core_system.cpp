#include "ui/core/ui_core_system.h"
#include "ui/core/ui_component.h"




namespace TheEngine::UI
{

    
    UICoreSystem::UICoreSystem(ECS::ECSEngine& ecsEngine, Memory::GPUBufferManager& gpuBufferManager, GPUTextureManager& gpuTextureManager) :
        m_ecsEngine(ecsEngine),
        m_uiVertexBufferManagementSystem(gpuBufferManager),
		m_gpuTextureManager(gpuTextureManager)
    {

        m_ecsEngine.registerComponent<UIRenderMeshComponent>();
        m_ecsEngine.registerComponent<UIMaterialComponent>();
        m_ecsEngine.registerComponent<UIRectDimensionsComponent>();
       // m_ecsEngine.registerComponent<UI::UIRectBoundsComponent>();

        //m_ecsEngine.registerComponent<UITransformComponent>();

        m_ecsEngine.registerComponent<UITextComponent>();
        m_ecsEngine.registerComponent<UITextMeshComponent>();
        m_ecsEngine.registerComponent<UIGraphComponent>();
        m_ecsEngine.registerComponent<UIRootComponent>();
       // m_ecsEngine.registerComponent<UIHierarchyComponent>();










    }

   
    ECS::ECSEngine& UICoreSystem::getECSEngine()
    {
      
        return m_ecsEngine;

    }



    UIVertexBufferManagementSystem& UICoreSystem::getUIVertexBufferManagementSystem()
    {
       

        return m_uiVertexBufferManagementSystem;

    }



}