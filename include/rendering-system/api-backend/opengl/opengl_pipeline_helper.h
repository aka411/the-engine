#pragma once
#include <glad/glad.h>
#include <rendering-system/low-level-gpu-systems/data-structures/gpu_pipeline_state_data_types.h>


namespace TheEngine::RenderingSystem::OpenGLBackend
{

//1) Rasterizer & Depth Helpers

 GLenum toGLCullMode(CullMode mode);
 GLenum toGLFillMode(FillMode mode);



//2. Blending Helpers

GLenum toGLCompareOp(CompareOp op);
GLenum toGLBlendFactor(BlendFactor factor);
GLenum toGLBlendOp(BlendOp op);




//3. Topology & Vertex Helpers
GLenum toGLTopology(PrimitiveTopology topology);







}