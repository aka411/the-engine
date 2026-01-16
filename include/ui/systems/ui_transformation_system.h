#pragma once
#include "ui/builder/ui_element.h"
#include "ui/core/ui_core_system.h"
#include "components.h"




namespace TheEngine::UI
{


	class UITransformationSystem
	{


	private:

		UICoreSystem& m_uiCoreSystem;

	public:


		UITransformationSystem(UICoreSystem& uiCoreSystem);

		EngineTransformationComponent& getTransformationComponent(ECS::EntityId& entityId);

		void buildLocalMatrix(EngineTransformationComponent& transform);
		
		void updateTransformation(ECS::EntityId rootEntityId);
		void updateTransformation(UIElement& rootUIElement);

	};


}