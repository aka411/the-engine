#pragma once
#include <vector>
#include <rendering-system/low-level-gpu-systems/gpu_shader_manager.h>
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu_memory_system_data_types.h>





namespace TheEngine::RenderingSystem
{

	enum class PrimitiveDataType
	{
		FLOAT
	};



	struct VertexAttribute
	{
		int location;
		int numOfComponents;
		bool normalize;
		int offset;

	};


	//can be dynamic
	class VertexLayout
	{
	private:

		
		std::vector< VertexAttribute> m_vertexAttributes;



	public:

		VertexLayout() = default;

	
		void addAttribute(const int location, const int numOfComponents, const PrimitiveDataType dataType, const bool normalise, const int offset)
		{

			VertexAttribute vertexAttribute;
			vertexAttribute.location = location;// in shader layout(location = 0)
			vertexAttribute.normalize = normalise;
			vertexAttribute.numOfComponents = numOfComponents;
			vertexAttribute.offset = offset;//in bytes

			m_vertexAttributes.push_back(vertexAttribute);
		};

		const std::vector<VertexAttribute>& getAttributes() const
		{
			return m_vertexAttributes;
		};


	};


	enum class PrimitiveTopology
	{
		TRIANGLES,
		LINES,
		POINTS,
		TRIANGLE_STRIP

	};





	enum class CullMode
	{
		NONE,
		BACK,
		FRONT,
		FRONT_AND_BACK
	};



	enum class CompareOp {
		NEVER,
		LESS,
		EQUAL,
		LESS_EQUAL,
		GREATER,
		NOT_EQUAL,
		GREATER_EQUAL,
		ALWAYS
	};

	enum class FillMode {
		SOLID,
		WIREFRAME,
		POINT
	};

	enum class BlendFactor {
		ZERO,
		ONE,
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		DST_COLOR,
		ONE_MINUS_DST_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA
	};

	enum class BlendOp {
		ADD,
		SUBTRACT,
		REVERSE_SUBTRACT,
		MIN,
		MAX
	};


	// --- Sub-structs for organization ---
	struct DepthStencilState {
		bool depthTestEnable = true;
		bool depthWriteEnable = true;
		CompareOp depthCompareOp = CompareOp::LESS;
		bool stencilTestEnable = false;
	};

	struct RasterizerState {
		CullMode cullMode = CullMode::BACK;
		FillMode fillMode = FillMode::SOLID;
		//float lineWidth = 1.0f;
	};

	struct BlendState {
		bool enabled = false;
		BlendFactor srcColorFactor = BlendFactor::SRC_ALPHA;
		BlendFactor dstColorFactor = BlendFactor::ONE_MINUS_SRC_ALPHA;
		BlendOp colorBlendOp = BlendOp::ADD;
	};

	struct PipelineDescriptor {
		ShaderProgram shader;
		VertexLayout layout;
		DepthStencilState depthStencil;
		RasterizerState rasterizer;
		BlendState blend;
	};





	class IGPUPipeline 
	{

	protected:

		PipelineDescriptor m_pipelineDescriptor;

	public:

		IGPUPipeline(PipelineDescriptor& pipelineDescriptor) :
			m_pipelineDescriptor(pipelineDescriptor)
		{
		
		};

		virtual ~IGPUPipeline() = default;
		
		virtual void bind() = 0;

	};



	using PipelineHandle = size_t;




	// Yeah this part i am not sure, like need more thought here


	struct BindingPointToBuffer
	{
		int location;
		GPUBufferInfo gpuBufferInfo;
	};

	struct BindingContext
	{
		std::vector<BindingPointToBuffer> vertexBuffers;
		std::vector<BindingPointToBuffer> UBOs;
		std::vector<BindingPointToBuffer> SSBOs;

	};







}