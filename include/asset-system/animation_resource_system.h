#pragma once
#include <vector>
#include <rendering-system/gpu-resource-system/gpu_animation_manager.h>



namespace ThEngine::RenderingSystem
{
	class GPUAnimationManager;
}


namespace TheEngine::AssetSystem
{

	/**-----  Skinned Animation Data  ----**/
	using InverseBindMatrixSetId = uint32_t;// uint32_t has max value of 4,294,967,295

	struct InverseBindMatrixSet
	{
		const std::vector<glm::mat4> inverseBindPoseMatrices;

		InverseBindMatrixSet(std::vector<glm::mat4>&& inverseBindPoseMatrices)
			: inverseBindPoseMatrices(std::move(inverseBindPoseMatrices))
		{

		}

	};


	/**-----  KeyFrame Animation Data (Actual Animation) ----**/

	using KeyframeAnimationSetId = uint32_t;


	enum class InterpolationMethod
	{
		LINEAR,
		STEP,
		CUBICSPLINE,
		UNKNOWN
		
		
	};

	enum class Path
	{
		TRANSLATION,
		ROTATION,
		SCALE
	};

	struct Channel
	{ 
		Path path;
		//TargetNode targetNode;// point of pain, this is link to external data
		//SamplerIndex samplerIndex;// Maybe not needed? 
	
	};

	struct Sampler
	{
		InterpolationMethod interpolationMethod;
		
		//Might need to use Index into another vector of keyframe times and values to avoid duplication
		std::vector<float> keyframeTimes;
		std::vector<float> keyframeValues;

		//size_t keyframeTimesIndex;
		//size_t keyframeValuesIndex;

	};

	/*
	
	struct KeyframeTimes
	{
		std::vector<float> keyframeTimes;
	};
		
	struct KeyframeValues
	{
		std::vector<float> keyframeValues;
	};
	
	*/



	struct KeyframeAnimation
	{
		std::vector<Channel> channels;
		std::vector<Sampler> samplers;

		//wait i have more doubt like maybe this will need to be in KeyframeAnimationSet as global
		//std::vector<KeyframeTimes> keyframeTimes;
		//std::vector<KeyframeValues> keyframeValues;

	};

	struct KeyframeAnimationSet
	{
		//string name to KeyframeAnimation
	};


	/*
	* There is a difference in how i used to store animation data in old design
	* the Channel and Sampler where stored in global animation vector where all animation data was stored.
	* 
	* 
	*/

	class AnimationResourceSystem
	{
	private:

	


		RenderingSystem::GPUAnimationManager& m_gpuAnimationManager;



		std::vector<KeyframeAnimationSet> m_keyframeAnimationSets;//need to track free slots also?

		std::vector<InverseBindMatrixSet> m_inverseBindMatrixSets;//need to track free slots also?


	public:


		AnimationResourceSystem(TheEngine::RenderingSystem::GPUAnimationManager& gpuAnimationManager);


		/**-------- KeyframeAnimation Animation Data Management--------**/

		KeyframeAnimationSetId storeKeyframeAimationSet(KeyframeAnimationSet&& keyframeAnimationSet);//stored in CPU memory,
		const KeyframeAnimationSet& getKeyframeAnimationSet(const KeyframeAnimationSetId keyframeAnimationSetId) const;//read only



		/**-------- Skinned Animation Data Management(Requires GPU Resource systems help also)--------**/

		// This is to store inverse bind pose matrices
		InverseBindMatrixSetId storeInverseBindMatrixSet(InverseBindMatrixSet&& inverseBindMatrixSet);//stored in CPU memory only,
		const InverseBindMatrixSet& getInverseBindMatrixSetId(const InverseBindMatrixSetId inverseBindMatrixSetId) const;//used by CPU side animation system,read only


		//Use stable Ids instead of offsets,stable ids allow for me to rearrange stuff in gpu memory
		//For MVP use offset then later we benchmark and try out stable ids


		//This two method requires coordination with GPUAnimationManager
		const RenderingSystem::BoneJointMatrixSetId uploadNewJointMatrixSetToGPUBuffer(std::vector<glm::mat4>&& jointMatrixSet);//One time registration method
		void updateJointMatrixSetInGPUBuffer(const  RenderingSystem::BoneJointMatrixSetId& boneJointMatrixSetId, std::vector<glm::mat4>&& jointMatrixSet);




	};



}