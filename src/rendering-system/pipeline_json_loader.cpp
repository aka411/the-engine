#include <rendering-system/pipeline_json_loader.h>
#define NLOHMANN_JSON_IMPLEMENTATION
#include "nlohmann/json.hpp"


namespace TheEngine::RenderingSystem
{
	using json = nlohmann::json;


	NLOHMANN_JSON_SERIALIZE_ENUM(CullMode, {
		{CullMode::NONE, "NONE"},
		{CullMode::FRONT, "FRONT"},
		{CullMode::BACK, "BACK"},
		{CullMode::FRONT_AND_BACK, "FRONT_AND_BACK"}
		})

		NLOHMANN_JSON_SERIALIZE_ENUM(FillMode, {
			{FillMode::SOLID, "SOLID"},
			{FillMode::WIREFRAME, "WIREFRAME"},
			{FillMode::POINT, "POINT"}
			})

		NLOHMANN_JSON_SERIALIZE_ENUM(FrontFace, {
			{FrontFace::CLOCKWISE, "CLOCKWISE"},
			{FrontFace::COUNTER_CLOCKWISE, "COUNTER_CLOCKWISE"}
			})

		NLOHMANN_JSON_SERIALIZE_ENUM(CompareOp, {
			{CompareOp::NEVER, "NEVER"},
			{CompareOp::LESS, "LESS"},
			{CompareOp::EQUAL, "EQUAL"},
			{CompareOp::LESS_OR_EQUAL, "LESS_OR_EQUAL"},
			{CompareOp::GREATER, "GREATER"},
			{CompareOp::NOT_EQUAL, "NOT_EQUAL"},
			{CompareOp::GREATER_OR_EQUAL, "GREATER_OR_EQUAL"},
			{CompareOp::ALWAYS, "ALWAYS"}
			})



		NLOHMANN_JSON_SERIALIZE_ENUM(StencilOp, {
			{StencilOp::KEEP, "KEEP"},
			{StencilOp::ZERO, "ZERO"},
			{StencilOp::REPLACE, "REPLACE"},
			{StencilOp::INCREMENT_AND_CLAMP, "INCREMENT_AND_CLAMP"},
			{StencilOp::DECREMENT_AND_CLAMP, "DECREMENT_AND_CLAMP"},
			{StencilOp::INVERT, "INVERT"},
			{StencilOp::INCREMENT_AND_WRAP, "INCREMENT_AND_WRAP"},
			{StencilOp::DECREMENT_AND_WRAP, "DECREMENT_AND_WRAP"}
			})

		NLOHMANN_JSON_SERIALIZE_ENUM(BlendFactor, {
			{BlendFactor::ZERO, "ZERO"},
			{BlendFactor::ONE, "ONE"},
			{BlendFactor::SRC_COLOR, "SRC_COLOR"},
			{BlendFactor::ONE_MINUS_SRC_COLOR, "ONE_MINUS_SRC_COLOR"},
			{BlendFactor::DST_COLOR, "DST_COLOR"},
			{BlendFactor::ONE_MINUS_DST_COLOR, "ONE_MINUS_DST_COLOR"},
			{BlendFactor::SRC_ALPHA, "SRC_ALPHA"},
			{BlendFactor::ONE_MINUS_SRC_ALPHA, "ONE_MINUS_SRC_ALPHA"}
			})


		NLOHMANN_JSON_SERIALIZE_ENUM(BlendOp, {
	{BlendOp::ADD, "ADD"},
	{BlendOp::SUBTRACT, "SUBTRACT"},
	{BlendOp::REVERSE_SUBTRACT, "REVERSE_SUBTRACT"},
	{BlendOp::MIN, "MIN"},
	{BlendOp::MAX, "MAX"}
			})



		NLOHMANN_JSON_SERIALIZE_ENUM(PrimitiveTopology, {
			{PrimitiveTopology::POINTS, "POINTS"},
			{PrimitiveTopology::LINES, "LINES"},
			{PrimitiveTopology::TRIANGLES, "TRIANGLES"},
			{PrimitiveTopology::TRIANGLE_STRIP, "TRIANGLE_STRIP"}
			})


		NLOHMANN_JSON_SERIALIZE_ENUM(SampleCountBit, {
	{ SampleCountBit::SAMPLE_COUNT_1_BIT,  "SAMPLE_COUNT_1_BIT"  },
	{ SampleCountBit::SAMPLE_COUNT_2_BIT,  "SAMPLE_COUNT_2_BIT"  },
	{ SampleCountBit::SAMPLE_COUNT_4_BIT,  "SAMPLE_COUNT_4_BIT"  },
	{ SampleCountBit::SAMPLE_COUNT_8_BIT,  "SAMPLE_COUNT_8_BIT"  },
	{ SampleCountBit::SAMPLE_COUNT_16_BIT, "SAMPLE_COUNT_16_BIT" },
	{ SampleCountBit::SAMPLE_COUNT_32_BIT, "SAMPLE_COUNT_32_BIT" },
	{ SampleCountBit::SAMPLE_COUNT_64_BIT, "SAMPLE_COUNT_64_BIT" }
			})


		void from_json(const json& j, StencilFaceState& s)
	{


		s.failOp = j.value("failOp", StencilOp::KEEP);
		s.passOp = j.value("passOp", StencilOp::KEEP);
		s.depthFailOp = j.value("depthFailOp", StencilOp::KEEP);
		s.compareOp = j.value("compareOp", CompareOp::ALWAYS);
		s.compareMask = j.value("compareMask", 0xFF);
		s.writeMask = j.value("writeMask", 0xFF);
		s.reference = j.value("reference", 0);
	}


	void from_json(const json& j, DepthStencilState& s)
	{
		s.depthTestEnable = j.value("depthTestEnable", true);
		s.depthWriteEnable = j.value("depthWriteEnable", true);
		s.depthCompareOp = j.value("depthCompareOp", CompareOp::LESS);
		s.stencilTestEnable = j.value("stencilTestEnable", false);
		if (j.contains("stencilFront")) j.at("stencilFront").get_to(s.stencilFront);
		if (j.contains("stencilBack"))  j.at("stencilBack").get_to(s.stencilBack);
	}


	void from_json(const json& j, RasterizerState& s)
	{
		s.cullMode = j.value("cullMode", CullMode::BACK);
		s.fillMode = j.value("fillMode", FillMode::SOLID);
		s.frontFace = j.value("frontFace", FrontFace::CLOCKWISE);
		s.lineWidth = j.value("lineWidth", 1.0f);
		s.depthBiasEnable = j.value("depthBiasEnable", false);
		s.depthBiasConstant = j.value("depthBiasConstant", 0.0f);
		s.depthBiasSlope = j.value("depthBiasSlope", 0.0f);
		s.depthBiasClamp = j.value("depthBiasClamp", 0.0f);

	}


	void from_json(const nlohmann::json& j, ColorAttachmentBlendState& s)
	{
		s.blendEnable = j.value("blendEnable", false);


		s.srcColorBlendFactor = j.value("srcColorFactor", BlendFactor::ONE);
		s.dstColorBlendFactor = j.value("dstColorFactor", BlendFactor::ZERO);
		s.colorBlendOp = j.value("colorBlendOp", BlendOp::ADD);
		s.srcAlphaBlendFactor = j.value("srcAlphaFactor", BlendFactor::ONE);
		s.dstAlphaBlendFactor = j.value("dstAlphaFactor", BlendFactor::ZERO);
		s.alphaBlendOp = j.value("alphaBlendOp", BlendOp::ADD);


		if (j.contains("colorWriteMask")) {
			auto val = j.at("colorWriteMask");

			if (val.is_string()) {
				std::string maskStr = val.get<std::string>();

				if (maskStr == "ALL" || maskStr == "RGBA") {
					s.colorWriteMask = ColorComponentFlags::ALL;
				}
				else if (maskStr == "NONE") {
					s.colorWriteMask = ColorComponentFlags::NONE;
				}
				else {

					uint8_t mask = 0;
					if (maskStr.find('R') != std::string::npos) mask |= (uint8_t)ColorComponentFlags::R;
					if (maskStr.find('G') != std::string::npos) mask |= (uint8_t)ColorComponentFlags::G;
					if (maskStr.find('B') != std::string::npos) mask |= (uint8_t)ColorComponentFlags::B;
					if (maskStr.find('A') != std::string::npos) mask |= (uint8_t)ColorComponentFlags::A;

					s.colorWriteMask = static_cast<ColorComponentFlags>(mask);
				}
			}
			else if (val.is_number()) {

				s.colorWriteMask = static_cast<ColorComponentFlags>(val.get<uint8_t>());
			}
		}
		else {
			s.colorWriteMask = ColorComponentFlags::ALL;
		}
	}


	void from_json(const json& j, MultisampleState& s)
	{

		s.alphaToCoverageEnable = j.value("alphaToCoverageEnable", false);
		s.alphaToOneEnable = j.value("alphaToOneEnable", false);
		s.minSampleShading = j.value("minSampleShading", 1.0f);
		s.sampleShadingEnable = j.value("sampleShadingEnable", false);


		if (j.contains("rasterizationSamples"))
		{
			s.rasterizationSamples = j.at("rasterizationSamples").get<SampleCountBit>();
		}
		else
		{
			s.rasterizationSamples = SampleCountBit::SAMPLE_COUNT_1_BIT;
		}
	}




	//The Master Config
	void from_json(const json& j, PipelineStateConfig& s)
	{
		if (j.contains("DepthStencilState")) j.at("DepthStencilState").get_to(s.depthStencilState);
		if (j.contains("RasterizerState"))   j.at("RasterizerState").get_to(s.rasterizerState);

		if (j.contains("ColorAttachmentBlendStates"))
		{
			const auto& node = j.at("ColorAttachmentBlendStates");

			if (node.is_array())
			{
				s.colorAttachmentBlendStateCount = static_cast<uint8_t>(std::min(node.size(), size_t(8)));
				for (size_t i = 0; i < s.colorAttachmentBlendStateCount; ++i)
				{


					s.colorAttachmentBlendStates[i] = node.at(i).get<ColorAttachmentBlendState>();
				}
			}
			else
			{
				// Single object applied to all
				ColorAttachmentBlendState commonBlend = node.get<ColorAttachmentBlendState>();
				for (auto& b : s.colorAttachmentBlendStates) {
					b = commonBlend;
				}
				s.colorAttachmentBlendStateCount = 1;
			}
		}





		s.topology = j.value("Topology", PrimitiveTopology::TRIANGLES);

		if (j.contains("MultisampleState"))   j.at("MultisampleState").get_to(s.multisampleState);

	}



}






namespace TheEngine::RenderingSystem
{
	using json = nlohmann::json;


	NLOHMANN_JSON_SERIALIZE_ENUM(ResourceFormat, {
		{ResourceFormat::UNKNOWN, "UNKNOWN"},

		// --- Standard Unorm Formats ---
		{ResourceFormat::R8_UNORM, "R8_UNORM"},
		{ResourceFormat::RG8_UNORM, "RG8_UNORM"},
		{ResourceFormat::RGBA8_UNORM, "RGBA8_UNORM"},
		{ResourceFormat::BGRA8_UNORM, "BGRA8_UNORM"},

		// --- Floats for Attributes ---
		{ResourceFormat::RG32_FLOAT, "RG32_FLOAT"},
		{ResourceFormat::RGB32_FLOAT, "RGB32_FLOAT"},

		// --- SRGB Formats ---
		{ResourceFormat::RGBA8_SRGB, "RGBA8_SRGB"},

		// --- Floating Point Formats (HDR) ---
		{ResourceFormat::R16_FLOAT, "R16_FLOAT"},
		{ResourceFormat::RG16_FLOAT, "RG16_FLOAT"},
		{ResourceFormat::RGBA16_FLOAT, "RGBA16_FLOAT"},
		{ResourceFormat::R32_FLOAT, "R32_FLOAT"},
		{ResourceFormat::RGBA32_FLOAT, "RGBA32_FLOAT"},

		// --- Special Packed Formats ---
		{ResourceFormat::R11G11B10_FLOAT, "R11G11B10_FLOAT"},
		{ResourceFormat::RGB10A2_UNORM, "RGB10A2_UNORM"},

		// --- Integer Formats ---
		{ResourceFormat::R32_UINT, "R32_UINT"},
		{ResourceFormat::RGBA32_UINT, "RGBA32_UINT"},

		// --- Depth and Stencil ---
		{ResourceFormat::D16_UNORM, "D16_UNORM"},
		{ResourceFormat::D32_FLOAT, "D32_FLOAT"},
		{ResourceFormat::D24_S8_UINT, "D24_S8_UINT"},
		{ResourceFormat::D32_S8_FLOAT, "D32_S8_FLOAT"}
		})






	void from_json(const json& j, VertexBinding& b)
	{
		b.binding = j.value("binding", 0);
		b.stride = j.value("stride", 0);
		b.isInstanced = j.value("isInstanced", false);
	}

	//Attribute Data
	void from_json(const json& j, VertexAttribute& a)
	{
		a.binding = j.value("binding", 0);
		a.location = j.value("location", 0);
		a.format = j.value("format", ResourceFormat::UNKNOWN);
		a.offset = j.value("offset", 0);
	}

	//The Layout Class 
	void from_json(const json& j, VertexLayout& layout)
	{
		layout.clear();

		if (j.contains("bindings") && j.at("bindings").is_array()) {
			for (auto& element : j.at("bindings")) {
				VertexBinding b = element.get<VertexBinding>();
				layout.addBinding(b.binding, b.stride, b.isInstanced);
			}
		}

		if (j.contains("attributes") && j.at("attributes").is_array()) {
			for (auto& element : j.at("attributes")) {
				VertexAttribute a = element.get<VertexAttribute>();
				layout.addAttribute(a.location, a.binding, a.format, a.offset);
			}
		}
	}









}





namespace TheEngine::RenderingSystem
{
	//Individual Attachment Format
	void from_json(const json& j, RenderTargetFormat& f)
	{
		f.colorFormat = j.value("colorFormat", ResourceFormat::RGBA8_UNORM);
		//f.alphaBlending = j.value("alphaBlending", false);
		f.scaleX = j.value("scaleX", 1.0f);
		f.scaleY = j.value("scaleY", 1.0f);
		f.sizeRelativeToWindow = j.value("sizeRelativeToWindow", true);
		f.sampleCount = j.value("sampleCount", 1);
	}

	//Full Output Config (The Framebuffer Layout)
	void from_json(const json& j, RenderOutputConfiguration& c)
	{

		if (j.contains("colorAttachments")) {
			c.colorAttachments = j.at("colorAttachments").get<std::vector<RenderTargetFormat>>();
		}

		if (j.contains("depthStencilAttachment")) {
			c.depthStencilAttachment = j.at("depthStencilAttachment").get<RenderTargetFormat>();
		}

		c.hasDepth = j.value("hasDepth", false);
		c.hasStencil = j.value("hasStencil", false);


	}



}