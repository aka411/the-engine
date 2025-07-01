#pragma once
#include "igpu_resource.h"

//cleaned and checked
namespace TheEngine::Graphics
{



	enum class BufferUsage
	{

		// Specific rendering uses

		VERTEX,           // Vertex data
		INDEX,            // Index data
		UNIFORM          // Uniform buffer object / Constant buffer
	};

	enum class BufferCPUAccess
	{
		GPU_ONLY,          // Data primarily resides on GPU, CPU does not need direct access
		CPU_WRITE_ONLY,     // CPU writes data, GPU reads it
		CPU_READ_ONLY,      // GPU writes data, CPU reads it
		CPU_READ_WRITE      // Both CPU and GPU read/write (e.g., for persistent mapping)
	};



	enum class MemoryAccessFlags : uint32_t
	{
		NONE = 0,
		PERSISTENT_MAPPING = (1 << 0), // Hint for persistent mapping if CpuWriteOnly/CpuReadWrite
		COHERENT_MAPPING = (1 << 1), // Data write will be immediately visible to gpu
		VOLATILE = (1 << 2), // Hint that data changes frequently and can be discarded (similar to STREAM)
		IMMUTABLE_SIZE = (1 << 3)  // Hint that size will not change after creation
	};

	struct BufferDescriptor //BufferCreateInfo //BufferDescriptor
	{
		BufferUsage usage; 
		BufferCPUAccess cpuAccess;
		MemoryAccessFlags memoryAccessFlag;


	};

	
	class IGPUBuffer : public IGPUResource
	{
	protected:
		IGPUBuffer() = default; // Protected constructor to prevent direct instantiation

	public:
		ResourceType getType() const override { return ResourceType::BUFFER; }

		virtual ~IGPUBuffer() = default;



		virtual uint64_t getSize() const = 0;
		
		virtual void* map(const uint64_t offset,const uint64_t size) = 0;
		virtual void unmap() = 0;
		virtual	void flushMappedRange(const uint64_t offset, const uint64_t size) = 0;



		virtual bool isCurrentlyMapped() const = 0;
		virtual bool isPersistentMapped() const = 0;
		virtual bool isCoherent() const = 0;

	

	};
}




