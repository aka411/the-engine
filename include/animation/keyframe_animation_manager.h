#pragma once
#include <vector>
#include "components.h"

namespace TheEngine::Animation
{


	class KeyframeAnimationManager
	{

	private:


		std::vector<AnimationCPUComponent>  m_animationCPUComponents;

	public:

		KeyframeAnimationManager() = default;

		AnimationCPUComponent& getAnimationCPUComponentFromId(KeyframeAnimationId keyframeAnimationId);

		KeyframeAnimationId storeAnimationCPUComponent(AnimationCPUComponent&& animationCPUComponent);



	};


}