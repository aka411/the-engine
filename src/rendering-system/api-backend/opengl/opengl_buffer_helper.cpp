#include <rendering-system/api-backend/opengl/opengl_buffer_helper.h>



namespace TheEngine::RenderingSystem::OpenGLBackend
{






    GLbitfield toGLbitfield(const MemoryFlags& memoryFlags)
    {
        GLbitfield openglBitField = 0;


        if (MemoryFlags::COHERENT && memoryFlags) { openglBitField |= GL_MAP_COHERENT_BIT; }
        if (MemoryFlags::CPU_VISIBLE && memoryFlags) { openglBitField |= GL_MAP_WRITE_BIT; }
        if (MemoryFlags::PERSISTENT_MAPPED && memoryFlags) { openglBitField |= GL_MAP_PERSISTENT_BIT; }

        return openglBitField;

    }

    GLenum toGLenum(const GPUBufferType& gpuBufferType)
    {




        switch (gpuBufferType)
        {
        case GPUBufferType::VERTEX:
            return GL_ARRAY_BUFFER;

        case GPUBufferType::INDEX:
            return GL_ELEMENT_ARRAY_BUFFER;

        case GPUBufferType::UNIFORM:
            return GL_UNIFORM_BUFFER;

        case GPUBufferType::STORAGE:
            return GL_SHADER_STORAGE_BUFFER;

        case GPUBufferType::INDIRECT:
            return GL_DRAW_INDIRECT_BUFFER;

        case GPUBufferType::STAGING:

            return GL_COPY_READ_BUFFER;

        case GPUBufferType::UNKNOWN:
        default:
            assert(false && "Unknown buffer type requested");
            return 0;
        }



    }



}
