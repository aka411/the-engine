#include <iostream>
#include <fstream>
#include <platform/file_system.h>
#include <cassert>


namespace TheEngine::Platform
{



	FileSystem::FileSystem()
	{



	}




	FileData FileSystem::readFile(const std::string& pathToFile)
	{

		// yeah this i will rewrite later


		FileData fileData;


		std::ifstream file(pathToFile, std::ios::binary | std::ios::ate);
		if (!file) return fileData;

		const std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);

		if (size > 0)
		{

			void* rawBuffer = std::malloc(static_cast<size_t>(size));

			if (rawBuffer)
			{

				if (file.read(reinterpret_cast<char*>(rawBuffer), size))
				{
					fileData.size = static_cast<size_t>(size);
					fileData.data = {
					
						reinterpret_cast<std::byte*>(rawBuffer),
						[](void* p) { std::free(p); }
					};
				}
				else
				{
					std::free(rawBuffer);
					assert(false && "Failed to read file data.");
				}
			}
		}
		else
		{
			assert(false && "File is empty or could not determine size.");
		}

		return fileData;

	}


}