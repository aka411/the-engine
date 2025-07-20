#pragma once
#include "igpu_render_system.h"

namespace TheEngine::Graphics
{

    class RenderSystem : public IGPURenderSystem
    {
    private:
        //there are some more things to hash out about the ownership of the submitted data
        std::vector<RenderComponent*> m_components;
        std::vector<UIRenderComponent*> m_uiComponents;


		IGPUResourceSystem& m_resourceSystem;
		IGPURenderDevice& m_renderDevice;


		IGPUDynamicBufferAllocator& m_cameraBufferAllocator;
		//IGPUDynamicBufferAllocator& m_lightBufferAllocator;
        IGPUDynamicBufferAllocator& m_transformationBufferAllocator;




    public:
		~RenderSystem() override = default;

        RenderSystem();

        virtual void initialize() override;

        virtual void SubmitRenderComponents(const std::vector<RenderComponent*>& components) override;

		virtual void SubmitUIRenderComponents(const std::vector<UIRenderComponent*>& uiComponents) override;





		virtual void RenderFrame() override;



    };

}