#include <glad/glad.h>
#include <rendering-system/api-backend/opengl/opengl_pipeline_helper.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{



	//1) Rasterizer & Depth Helpers




	 GLenum toGLCullMode(CullMode mode) {
		switch (mode) {
		case CullMode::NONE:           return GL_NONE; // Handled by glDisable(GL_CULL_FACE)
		case CullMode::BACK:           return GL_BACK;
		case CullMode::FRONT:          return GL_FRONT;
		case CullMode::FRONT_AND_BACK: return GL_FRONT_AND_BACK;
		default: return GL_BACK;
		}
	}

	 GLenum toGLFillMode(FillMode mode) {
		switch (mode) {
		case FillMode::SOLID:          return GL_FILL;
		case FillMode::WIREFRAME:      return GL_LINE;
		case FillMode::POINT:          return GL_POINT;
		default: return GL_FILL;
		}
	}


	//2. Blending Helpers

	 GLenum toGLCompareOp(CompareOp op) {
		switch (op) {
		case CompareOp::NEVER:         return GL_NEVER;
		case CompareOp::LESS:          return GL_LESS;
		case CompareOp::EQUAL:         return GL_EQUAL;
		case CompareOp::LESS_EQUAL:    return GL_LEQUAL;
		case CompareOp::GREATER:       return GL_GREATER;
		case CompareOp::NOT_EQUAL:     return GL_NOTEQUAL;
		case CompareOp::GREATER_EQUAL: return GL_GEQUAL;
		case CompareOp::ALWAYS:        return GL_ALWAYS;
		default: return GL_LESS;
		}
	}





	 GLenum toGLBlendFactor(BlendFactor factor) {
		switch (factor) {
		case BlendFactor::ZERO:                return GL_ZERO;
		case BlendFactor::ONE:                 return GL_ONE;
		case BlendFactor::SRC_COLOR:           return GL_SRC_COLOR;
		case BlendFactor::ONE_MINUS_SRC_COLOR: return GL_ONE_MINUS_SRC_COLOR;
		case BlendFactor::DST_COLOR:           return GL_DST_COLOR;
		case BlendFactor::ONE_MINUS_DST_COLOR: return GL_ONE_MINUS_DST_COLOR;
		case BlendFactor::SRC_ALPHA:           return GL_SRC_ALPHA;
		case BlendFactor::ONE_MINUS_SRC_ALPHA: return GL_ONE_MINUS_SRC_ALPHA;
		case BlendFactor::DST_ALPHA:           return GL_DST_ALPHA;
		case BlendFactor::ONE_MINUS_DST_ALPHA: return GL_ONE_MINUS_DST_ALPHA;
		default: return GL_ONE;
		}




	}

	 GLenum toGLBlendOp(BlendOp op) {
		switch (op) {
		case BlendOp::ADD:              return GL_FUNC_ADD;
		case BlendOp::SUBTRACT:         return GL_FUNC_SUBTRACT;
		case BlendOp::REVERSE_SUBTRACT: return GL_FUNC_REVERSE_SUBTRACT;
		case BlendOp::MIN:              return GL_MIN;
		case BlendOp::MAX:              return GL_MAX;
		default: return GL_FUNC_ADD;
		}
	}

	//3. Topology & Vertex Helpers
	 GLenum toGLTopology(PrimitiveTopology topology) {
		switch (topology) {
		case PrimitiveTopology::TRIANGLES:      return GL_TRIANGLES;
		case PrimitiveTopology::LINES:          return GL_LINES;
		case PrimitiveTopology::POINTS:         return GL_POINTS;
		case PrimitiveTopology::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
		default: return GL_TRIANGLES;
		}
	}






}