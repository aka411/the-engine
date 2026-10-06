#pragma once
#include <vector>
#include <cstddef>

namespace TheEngine::Platform
{


	class File
	{

	private:

		std::vector<std::byte> m_buffer;   
		//std::unique_ptr<IMappedFile> m_mappedFile;

	public:

		File() = default;
		//File(const File&) = delete;

		//File& operator= (const File&) = delete;
		//TODO

		//File(File&& other) noexcept;
		//File& operator=(File&& other) noexcept;



		static File createFromPointer(const std::byte* dataPtr, const size_t size);
		static File createFromBuffer(std::vector<std::byte>&& buffer);
		//static File createFromMappedFile(std::unique_ptr<IMappedFile> mappedFile);

		const size_t size() const;

		std::byte* data();
		std::byte* begin();
		std::byte* end();

		const std::byte* data() const;
		const std::byte* begin() const;
		const std::byte* end() const;
	};




}