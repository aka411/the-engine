#pragma once
#include <glad/glad.h>



namespace TheEngine::RenderingSystem::OpenGLBackend
{



    GLbitfield toGLbitfield(const MemoryFlags& memoryFlags);

    GLenum toGLenum(const BufferType& gpuBufferType);



}