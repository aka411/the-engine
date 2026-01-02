
#include <unordered_map>
#include "tinygltf/tiny_gltf.h"
#include "animation/animation_data_structures.h"
#include "model-loading/gltf_animation_extractor.h"

namespace TheEngine
{


	Animation::AnimationPathType GltfAnimationExtractor::getAnimationType(const std::string& animationPath)
	{

		if (animationPath == "translation")
		{
			return Animation::AnimationPathType::TRANSLATION;
		}
		if (animationPath == "scale")
		{
			return Animation::AnimationPathType::SCALE;
		}
		if (animationPath == "rotation")
		{
			return Animation::AnimationPathType::ROTATION;
		}

		return Animation::AnimationPathType::UNKNOWN;
	}



	Animation::AnimationInterpolationMode GltfAnimationExtractor::getAnimationInterpolationMode(const std::string& tinygltfAnimationInterpolationMode)
	{

		if (tinygltfAnimationInterpolationMode == "LINEAR")
		{
			return Animation::AnimationInterpolationMode::LINEAR;
		}
		if (tinygltfAnimationInterpolationMode == "STEP")
		{
			return Animation::AnimationInterpolationMode::STEP;
		}
		if (tinygltfAnimationInterpolationMode == "CUBICSPLINE")
		{
			return Animation::AnimationInterpolationMode::CUBICSPLINE;
		}

		return Animation::AnimationInterpolationMode::UNKNOWN;
	}




	 ExtractedAnimation GltfAnimationExtractor::extractAnimation(const tinygltf::Model& tinygltfModel)
	{  //gltf Sampler accessor Index , Engine Sampler Index


		ExtractedAnimation extractedAnimation;




		/*COVERS EVERY ANIMATION*/
		std::unordered_map<int, int> inputIndexMap;
		std::unordered_map<int, int> outputIndexMap;


		std::vector< Animation::AnimationTimeInput> inputList;
		std::vector< Animation::AnimationOutputValue> outputList;

		if (tinygltfModel.animations.size() != 0)
		{
			extractedAnimation.hasAnimations = true;
		}

		for (auto& animation : tinygltfModel.animations)
		{
			extractedAnimation.animationsMap[animation.name] = Animation::Animation{}; //create empty animation entry
			float maxTime = 0.0f;

			std::unordered_map<int, int> samplerIndexMap; /*PER ANIMATION MAP*/




			std::vector<Animation::AnimationChannel> animationChannelList;
			animationChannelList.reserve(animation.channels.size());

			//CREATE SAMPLER VECTOR
			std::vector<Animation::AnimationSampler> animationSamplerList;
			animationSamplerList.reserve(animation.samplers.size());

			for (auto& channel : animation.channels)
			{

				Animation::AnimationChannel animationChannel;

				animationChannel.targetNodeIndex = channel.target_node;
				animationChannel.pathType = getAnimationType(channel.target_path);

				assert(animationChannel.pathType != Animation::AnimationPathType::UNKNOWN);

				assert(channel.sampler < animation.samplers.size());

				const int gltfSamplerIndex = channel.sampler;
				//using map to find 
				if (samplerIndexMap.find(gltfSamplerIndex) == samplerIndexMap.end())
				{
					//need new sampler
					Animation::AnimationSampler animationSampler;
					animationSampler.interpolationMode = getAnimationInterpolationMode(animation.samplers[gltfSamplerIndex].interpolation);





					//For input
					const int gltfInputIndex = animation.samplers[gltfSamplerIndex].input;
					if (inputIndexMap.find(gltfInputIndex) == inputIndexMap.end())
					{
						//need to create new input
						const tinygltf::Accessor& inputAccessor = tinygltfModel.accessors[gltfInputIndex];
						const tinygltf::BufferView& inputBufferView = tinygltfModel.bufferViews[inputAccessor.bufferView];
						const tinygltf::Buffer& inputBuffer = tinygltfModel.buffers[inputBufferView.buffer];

						size_t inputByteOffset = inputBufferView.byteOffset + inputAccessor.byteOffset;
						size_t inputCount = inputAccessor.count;

						std::vector<float> inputKeyframes;
						inputKeyframes.resize(inputCount);

						memcpy(inputKeyframes.data(), inputBuffer.data.data() + inputByteOffset, inputCount * sizeof(float));

						Animation::AnimationTimeInput animationTimeInput;
						animationTimeInput.inputKeyframes = inputKeyframes;

						animationSampler.inputIndex = static_cast<int>(inputList.size());//new input index

						inputList.push_back(animationTimeInput);

						inputIndexMap[gltfInputIndex] = animationSampler.inputIndex;


						if (inputKeyframes.back() > maxTime)
						{
							maxTime = inputKeyframes.back();
						}


					}
					else
					{
						const int engineInputIndex = inputIndexMap.find(gltfInputIndex)->second;
						animationSampler.inputIndex = engineInputIndex;

						//update max time
						if (inputList[engineInputIndex].inputKeyframes.back() > maxTime)
						{
							maxTime = inputList[engineInputIndex].inputKeyframes.back();
						}
					}





					//For output
					const int gltfOutputIndex = animation.samplers[gltfSamplerIndex].output;
					if (outputIndexMap.find(gltfOutputIndex) == outputIndexMap.end())
					{
						//need to create new output
						const tinygltf::Accessor& outputAccessor = tinygltfModel.accessors[gltfOutputIndex];
						const tinygltf::BufferView& outputBufferView = tinygltfModel.bufferViews[outputAccessor.bufferView];
						const tinygltf::Buffer& outputBuffer = tinygltfModel.buffers[outputBufferView.buffer];
						size_t outputByteOffset = outputBufferView.byteOffset + outputAccessor.byteOffset;
						size_t outputCount = outputAccessor.count;
						size_t typeCount = 0;
						if (animationChannel.pathType == Animation::AnimationPathType::TRANSLATION || animationChannel.pathType == Animation::AnimationPathType::SCALE)
						{
							typeCount = 3;
						}
						else if (animationChannel.pathType == Animation::AnimationPathType::ROTATION)
						{
							typeCount = 4;
						}

						std::vector<float> outputValues;
						outputValues.resize(outputCount * typeCount);

						memcpy(outputValues.data(), outputBuffer.data.data() + outputByteOffset, outputCount * typeCount * sizeof(float));

						Animation::AnimationOutputValue animationOutputValue;
						animationOutputValue.outputValues = outputValues;

						animationSampler.outputIndex = static_cast<int>(outputList.size());//new output index

						outputList.push_back(animationOutputValue);

						outputIndexMap[gltfOutputIndex] = animationSampler.outputIndex;



					}
					else
					{
						const int engineOutputIndex = outputIndexMap.find(gltfOutputIndex)->second;
						animationSampler.outputIndex = engineOutputIndex;

					}


					animationChannel.samplerIndex = static_cast<int>(animationSamplerList.size());//new sampler index
					samplerIndexMap[gltfSamplerIndex] = animationChannel.samplerIndex;
					animationSamplerList.push_back(animationSampler);

				}
				else
				{
					const int engineSamplerIndex = samplerIndexMap.find(gltfSamplerIndex)->second;
					animationChannel.samplerIndex = engineSamplerIndex;
				}


				animationChannelList.push_back(animationChannel);



			}

			Animation::Animation engineAnimation;
			engineAnimation.animationChannel = animationChannelList;
			engineAnimation.animationSamplers = animationSamplerList;
			engineAnimation.maxTime = maxTime;

			extractedAnimation.animationsMap[animation.name] = engineAnimation;

		}


		extractedAnimation.animationData.inputs= inputList;
		extractedAnimation.animationData.outputs = outputList;
		
		return extractedAnimation;
	}










	Animation::BoneAnimationData GltfAnimationExtractor::getBoneAnimationData(const tinygltf::Model& tinygltfModel)
	{
		const std::vector<tinygltf::Accessor>& accessors = tinygltfModel.accessors;
		const std::vector<tinygltf::BufferView>& bufferViews = tinygltfModel.bufferViews;

		Animation::BoneAnimationData boneAnimationData;

		if (tinygltfModel.skins.size() == 0 /**!(tinygltfModel.skins[0].inverseBindMatrices >= 0)*/)
		{
			boneAnimationData.isSkinned = false;
			return boneAnimationData;
		}

		if (!(bufferViews[accessors[tinygltfModel.skins[0].inverseBindMatrices].bufferView].byteLength == boneAnimationData.inverseBindMatrices.size() * sizeof(glm::mat4)))
		{
			boneAnimationData.isSkinned = false;
			return boneAnimationData;
		}


		boneAnimationData.isSkinned = true;

		boneAnimationData.jointIndices = tinygltfModel.skins[0].joints;

		boneAnimationData.inverseBindMatrices.resize(boneAnimationData.jointIndices.size());

		boneAnimationData.jointMatrices.resize(boneAnimationData.jointIndices.size(), glm::mat4(1.0f));



		const std::vector<unsigned char>& buffer = tinygltfModel.buffers[0].data;

		const size_t srcIndex = accessors[tinygltfModel.skins[0].inverseBindMatrices].byteOffset + bufferViews[accessors[tinygltfModel.skins[0].inverseBindMatrices].bufferView].byteOffset;
		
		//Note:
		//I need to look into gltf spec to see why byte length in buffer view is not equal to count * sizeof(mat4)
		//in a model i tested with, i assumption is that it was either padding or corruption
		//most probably its wrong data in the gltf file or may be issue in tinygltf loader
		assert(bufferViews[accessors[tinygltfModel.skins[0].inverseBindMatrices].bufferView].byteLength == boneAnimationData.inverseBindMatrices.size() * sizeof(glm::mat4));
		//memcpy(boneAnimationData.inverseBindMatrices.data(), &buffer[srcIndex], bufferViews[accessors[tinygltfModel.skins[0].inverseBindMatrices].bufferView].byteLength); // unsafe casued a heap corruption in one model i tested with
		memcpy(boneAnimationData.inverseBindMatrices.data(), &buffer[srcIndex], boneAnimationData.inverseBindMatrices.size() * sizeof(glm::mat4));


		return boneAnimationData;
	}





}