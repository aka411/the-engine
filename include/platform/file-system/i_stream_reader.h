#pragma once

namespace TheEngine::Platform
{

	class IStreamReader
	{
	private:


	public:

		virtual ~IStreamReader() = default;

		virtual size_t size() const = 0;
		virtual size_t read(void* destBuffer, const size_t size, const size_t offset) = 0;
	};


}