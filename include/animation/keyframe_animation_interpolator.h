#pragma once
#include <vector>
#include <glm/fwd.hpp>
#include "animation_data_structures.h"
#include "../components.h"


namespace TheEngine
{

	class KeyframeAnimationInterpolator
	{

	private:
		struct LinearData
		{
			size_t currentFrameIndex = 0;
			size_t nextFrameIndex = 0;
		};

		static int step(const std::vector<float>& input, const float currentTime);
		static LinearData linear(const std::vector<float>& input, const float currentTime);


		static glm::vec3 animateTranslate(TheEngine::AnimationInterpolationMode animationinterpolationMode, const float totalDuration, const std::vector<float>& input, const std::vector<float>& output, float currentTime);
		static glm::quat animateRotate(TheEngine::AnimationInterpolationMode animationinterpolationMode, const float totalDuration, const std::vector<float>& input, const std::vector<float>& output, float currentTime);

	public:

		KeyframeAnimationInterpolator();


		static void animate(AnimationStateComponent& animationStateComponent, const AnimationCPUComponent& animationCPUComponent, std::vector<EngineTransformationComponent*>& engineTransformationComponents, const float deltaTime);

	};


}
