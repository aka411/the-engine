#include "../../../include/igpu_resource.h"
#include "../the-engine/graphics/include/backends/opengl/opengl_buffer.h"
#include <stdexcept>


//cleaned and checked
namespace TheEngine::Graphics
{




    OpenGLBuffer::OpenGLBuffer(const BufferDescriptor& bufferDescriptor,const GLuint id,const GLenum target,const uint64_t size)
        :m_bufferDescriptor(bufferDescriptor), m_id(id),m_target(target),m_size(size)
    {


        m_isCoherent=(m_bufferDescriptor.memoryAccessFlag & MemoryAccessFlags::COHERENT_MAPPING);
        m_isPersistentMapped = (m_bufferDescriptor.memoryAccessFlag & MemoryAccessFlags::PERSISTENT_MAPPING);
        
        if (m_isPersistentMapped)
        {
            m_mappedPtr=map(0, m_size);
         
        }


    }

    uint64_t OpenGLBuffer::getSize() const
    {
        return m_size;
    }


  


    void* OpenGLBuffer::map(const uint64_t offset, const uint64_t size)
    {
		//vulkan cannot do this if buffer is not host visible, 
        // opengl can with glMapBufferRange but under the hood it will be slow and synchronization may be required.

        if (m_isCurrentlyMapped)
        { // Already mapped (e.g., persistently)
            if (m_isPersistentMapped)
            {
                return static_cast<uint8_t*>(m_mappedPtr) + offset;
            }
            else
            {
                throw std::runtime_error("Buffer is already temporarily mapped. Call Unmap() first.");
            }
        }

        glBindBuffer(m_target, m_id);

        GLbitfield mapFlags = 0;

        if (m_bufferDescriptor.cpuAccess == BufferCPUAccess::CPU_WRITE_ONLY || m_bufferDescriptor.cpuAccess == BufferCPUAccess::CPU_READ_WRITE)
        {
            mapFlags |= GL_MAP_WRITE_BIT;
        }
        if (m_bufferDescriptor.cpuAccess == BufferCPUAccess::CPU_READ_ONLY || m_bufferDescriptor.cpuAccess == BufferCPUAccess::CPU_READ_WRITE)
        {
            mapFlags |= GL_MAP_READ_BIT;
        }

        // Persistent and Coherent flags are handled in constructor for glBufferStorage
        // This Map call is for *temporary* mapping of mutable buffers or if persistent was not chosen for immutable.

        m_mappedPtr = glMapBufferRange(m_target, offset, size, mapFlags);

        if (!m_mappedPtr)
        {
            throw std::runtime_error("Failed to map OpenGL buffer range.");
        }


        m_isCurrentlyMapped = true; // Mark as currently mapped

        glBindBuffer(m_target, 0); // Unbind after map, as per common practice

        return m_mappedPtr; // Return the full pointer for the mapped range
    }

    void OpenGLBuffer::unmap()
    {
        if (!m_isCurrentlyMapped || m_isPersistentMapped)
        { // Not mapped or persistently mapped
            if (m_isPersistentMapped)
            {
                // No explicit unmap for persistent mapping
                return;
            }
            throw std::runtime_error("Buffer is not currently mapped.");
        }

        glBindBuffer(m_target, m_id);

        if (glUnmapBuffer(m_target) == GL_FALSE)
        {
            // Handle error: contents corrupted, e.g., by hardware reset
            // You might need to re-upload data here in a real engine
            throw std::runtime_error("OpenGL buffer unmap failed (contents corrupted).");
        }

        glBindBuffer(m_target, 0);

        m_mappedPtr = nullptr;
        m_isCurrentlyMapped = false;
    }



    bool OpenGLBuffer::isCurrentlyMapped() const
    {
        return m_isCurrentlyMapped;

        //check


    }

    bool OpenGLBuffer::isPersistentMapped() const
    {
        return m_isPersistentMapped;
    }

    bool OpenGLBuffer::isCoherent() const
    {
        return m_isCoherent;
    }


    void OpenGLBuffer::flushMappedRange(const uint64_t offset,const uint64_t size)
    {
        if (!m_isCurrentlyMapped || !m_mappedPtr)
        {
            throw std::runtime_error("Cannot flush unmapped buffer.");
        }
        // Only flush if not coherent (GL_MAP_COHERENT_BIT was NOT set)
        if (!(m_isCoherent))
        {
            glBindBuffer(m_target, m_id);
            glFlushMappedBufferRange(m_target, offset, size);
            glBindBuffer(m_target, 0);
        }
        // If coherent, no explicit flush is needed.
    }


    GLuint OpenGLBuffer::GetNativeHandle() const
    {
        return m_id;
    }

    OpenGLBuffer::~OpenGLBuffer()
    {
        if (m_id != 0) // Defensive check
        {
            // No explicit unmap for persistent mapping, as it's unmapped on buffer deletion.
            // Only unmap temporary mappings if somehow still active.
            if (m_isCurrentlyMapped && !m_isPersistentMapped)
            {
                // Unmap before deletion to avoid memory leaks/issues.
                // In a proper shutdown, this should ideally not be needed if Unmap() was called.
                // But as a safety measure for unexpected program termination.
                glBindBuffer(m_target, m_id);
                glUnmapBuffer(m_target); // Don't care about return value for shutdown
                glBindBuffer(m_target, 0);
            }
            glDeleteBuffers(1, &m_id);
            m_id = 0; // Prevent double deletion
        }
    }

} // namespace TheEngine::Graphics