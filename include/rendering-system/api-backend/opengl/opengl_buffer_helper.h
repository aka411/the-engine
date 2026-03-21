#pragma once
#include <glad/glad.h>

#include <assert.h>
#include <rendering-system/low-level-gpu-systems/gpu-memory-management/gpu_memory_system_data_types.h>

namespace TheEngine::RenderingSystem::OpenGLBackend
{



    GLbitfield toGLbitfield(const MemoryFlags& memoryFlags);

    GLenum toGLenum(const GPUBufferType& gpuBufferType);



}