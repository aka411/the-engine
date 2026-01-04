#pragma once
#include <unordered_map>
#include "tinygltf/tiny_gltf_wrapper.h"
#include "animation/animation_data_structures.h"
#include <string>



namespace TheEngine
{

	struct ExtractedAnimation
	{
		Animation::AnimationData animationData;
		std::unordered_map<std::string, Animation::Animation> animationsMap;
		bool hasAnimations = false;
	};




	class GltfAnimationExtractor
	{
	private:

		static Animation::AnimationPathType getAnimationType(const std::string& animationPath);

		static Animation::AnimationInterpolationMode getAnimationInterpolationMode(const std::string& tinygltfAnimationInterpolationMode);

	public:



		static ExtractedAnimation extractAnimation(const tinygltf::Model& tinygltfModel);

		static Animation::BoneAnimationData getBoneAnimationData(const tinygltf::Model& tinygltfModel);


	};

}