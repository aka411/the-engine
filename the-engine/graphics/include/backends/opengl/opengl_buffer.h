#pragma once
#include "../../igpu_buffer.h"
#include <cstdint>
#include "../../igpu_command_buffer.h"

////cleaned and checked
namespace TheEngine::Graphics
{
	class OpenGLBuffer : public IGPUBuffer
	{
	private:

		GLuint m_id;
		GLenum m_target; // e.g., GL_ARRAY_BUFFER, GL_UNIFORM_BUFFER

		void* m_mappedPtr = nullptr;

		uint64_t m_size;

	   const BufferDescriptor m_bufferDescriptor; // Store descriptor to know how it was created

		
		bool m_isCurrentlyMapped = false;

		bool m_isPersistentMapped = false; // True if created with GL_MAP_PERSISTENT_BIT
		bool m_isCoherent = false;

	public:
	

		OpenGLBuffer(const BufferDescriptor& bufferDescriptor,const GLuint id,const GLenum target,const uint64_t size);

		~OpenGLBuffer();

		virtual uint64_t getSize() const override;

		GLuint GetNativeHandle() const;
		
		virtual bool isCurrentlyMapped() const override;
		virtual bool isPersistentMapped() const override;
		virtual bool isCoherent() const override;

		virtual	void flushMappedRange(const uint64_t offset, const uint64_t size)override;
		virtual void* map(const uint64_t offset, const uint64_t size) override;
		virtual void unmap() override;

		

	
	};
}