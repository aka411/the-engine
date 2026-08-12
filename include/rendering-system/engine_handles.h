#pragma once
#include <cstdint>
#include <assert.h>
#include <limits>

namespace TheEngine::RenderingSystem
{



	constexpr uint64_t INVALID_RESOURCE_ID = std::numeric_limits<uint64_t>::max();
	constexpr uint64_t SPECIAL_VALID_RESOURCE_ID = INVALID_RESOURCE_ID - 1;


	template <typename Tag>
	struct ResourceHandle
	{
		uint64_t id = INVALID_RESOURCE_ID;


		ResourceHandle() = default;
		explicit ResourceHandle(const uint64_t _id) : id(_id) {}

		[[nodiscard]] bool isValid() const { return id != INVALID_RESOURCE_ID; }


		bool operator==(const ResourceHandle& other) const { return id == other.id; }
		bool operator!=(const ResourceHandle& other) const { return !(*this == other); }
	};


	using TextureHandle = ResourceHandle<struct TextureTag>;
	using BufferHandle = ResourceHandle<struct BufferTag>;
	using ShaderHandle = ResourceHandle<struct ShaderTag>;
	using SamplerHandle = ResourceHandle<struct SamplerTag>;
	using PipelineHandle = ResourceHandle<struct PipelineTag>;






	const TextureHandle SWAP_CHAIN_IMAGE_TEXTURE_HANDLE{ SPECIAL_VALID_RESOURCE_ID };



}