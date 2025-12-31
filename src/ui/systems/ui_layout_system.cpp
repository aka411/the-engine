#include "ui/systems/ui_layout_system.h"
#include "ui/core/ui_component.h"

namespace TheEngine::UI
{


    UILayoutSystem::UILayoutSystem(UICoreSystem& uiCoreSystem) : m_uiCoreSystem(uiCoreSystem), m_uiTransformationSystem(uiCoreSystem)
    {

    }

    void UILayoutSystem::updateLayout()
    {
        ECS::ECSEngine& ecsEngine = m_uiCoreSystem.getECSEngine();

        ECS::Query query = ecsEngine.getQuery<UIRootComponent>();

        //Consider a active component to tag active ui tree


        for (auto& chunkArrayView : query.getChunkArrayViews())
        {
            const size_t count = chunkArrayView.getCount();
         

            const ECS::EntityId* chunkRecordArray = chunkArrayView.getChunkRecordArray();

            for (size_t i = 0; i < count; ++i)
            {
               


                m_uiTransformationSystem.updateTransformation(chunkRecordArray[i]);





            }
        }





    }
}