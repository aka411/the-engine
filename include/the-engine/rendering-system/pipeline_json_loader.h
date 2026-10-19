#pragma once
#include <nlohmann/json.hpp>
#include <rendering-system/rhi/data-structures/gpu_types.h>
#include <rendering-system/rhi/data-structures/gpu_pipeline_data_structures.h>
#include <rendering-system/rhi/data-structures/vertex_layout.h>

namespace TheEngine::RenderingSystem
{
	

	//Stencil State
	void from_json(const nlohmann::json& j, StencilFaceState& s);


	//Depth/Stencil State
	void from_json(const nlohmann::json& j, DepthStencilState& s);

	//Rasterizer State
	void from_json(const nlohmann::json& j, RasterizerState& s);


	//Blend State
	void from_json(const nlohmann::json& j, ColorAttachmentBlendState& s);



	//The Master Config
	void from_json(const nlohmann::json& j, PipelineStateConfig& s);




}





//VERTEX 


namespace TheEngine::RenderingSystem
{




	void from_json(const nlohmann::json& j, VertexBinding& b);
	void from_json(const nlohmann::json& j, VertexAttribute& a);
	void from_json(const nlohmann::json& j, VertexLayout& layout);


}



namespace TheEngine::RenderingSystem
{
	void from_json(const nlohmann::json& j, RenderTargetFormat& f);
	void from_json(const nlohmann::json& j, RenderOutputConfiguration& c);
}