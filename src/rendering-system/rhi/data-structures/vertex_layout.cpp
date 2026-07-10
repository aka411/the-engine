#include <rendering-system/rhi/data-structures/vertex_layout.h>
#include <assert.h>




namespace TheEngine::RenderingSystem
{

	bool isVertexFormat(ResourceFormat format)
	{
		switch (format) {
		case ResourceFormat::D16_UNORM:
		case ResourceFormat::D32_FLOAT:
		case ResourceFormat::D24_S8_UINT:
		case ResourceFormat::D32_S8_FLOAT:
		case ResourceFormat::RGBA8_SRGB: 
			return false;
		default:
			return true;
		}
	}





	void VertexLayout::addBinding(uint32_t binding, uint32_t stride, bool isInstanced)
	{
		m_bindings.push_back({ binding, stride, isInstanced });
	}


	void VertexLayout::addAttribute( uint32_t binding, uint32_t location, ResourceFormat format, uint32_t offset)
	{
		//check if format comaptible
		assert(isVertexFormat(format) && "Inavlid Vertex Format");
		m_attributes.push_back({ binding, location, format, offset });
	}


	// Getters 
	const std::vector<VertexAttribute>& VertexLayout::getAttributes() const
	{
		return m_attributes;
	}

	const std::vector<VertexBinding>& VertexLayout::getBindings() const
	{
		return m_bindings;
	}

	// Helper to clear layout if reusing the object
	void VertexLayout::clear()
	{
		m_attributes.clear();
		m_bindings.clear();
	}






}