#pragma once


namespace TheEngine::RenderingSystem
{
	//IS NEWEST DESIGN
	enum class VertexComponentType
	{
		FLOAT,
		INT,
		BYTE,
		UNSIGNED_BYTE
	};




	struct VertexAttribute
	{
		uint32_t location;   // Shader layout(location = X)
		uint32_t binding;    // <--- THE BRIDGE: Which binding slot does this attribute use?
		uint32_t count;      // e.g., 3 for vec3
		VertexComponentType type;
		bool normalized;
		uint32_t offset;     // Offset within that specific buffer
	};


	struct VertexBinding
	{
		uint32_t binding;    // The Slot ID (0, 1, 2...)
		uint32_t stride;     // Bytes between vertices in THIS buffer
		bool isInstanced;    // Per-vertex or Per-instance
	};



	class VertexLayout
	{
	private:

		std::vector<VertexAttribute> m_attributes;
		std::vector<VertexBinding> m_bindings;

		uint32_t stride;

	public:

		void addBinding(uint32_t binding, uint32_t stride, bool isInstanced = false);
		void addAttribute(uint32_t location, uint32_t binding, uint32_t count, VertexComponentType type, bool norm, uint32_t offset)
		{
			m_attributes.push_back({ location, binding, count, type, norm, offset });
		}

		const std::vector<VertexAttribute>& getAttributes() const { return m_attributes; }
	};



}