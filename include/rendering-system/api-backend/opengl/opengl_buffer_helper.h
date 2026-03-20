#pragma once
#include <glad/glad.h>
#include <rendering-system/low-level-gpu-systems/memory-management/memory_system_data_types.h>
#include <assert.h>


namespace TheEngine::RenderingSystem::OpenGLBackend
{



    GLbitfield toGLbitfield(const MemoryFlags& memoryFlags);

    GLenum toGLenum(const GPUBufferType& gpuBufferType);



}