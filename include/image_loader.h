#pragma once
#include <rendering-system/low-level-gpu-systems/data-structures/gpu_texture_data_structures.h>



namespace TheEngine::Platform
{
	class Path;
	class FileSystem;
}


namespace TheEngine
{


	class ImageLoader
	{
	private:

		TheEngine::Platform::FileSystem& m_fileSystem;

	public:

		ImageLoader(TheEngine::Platform::FileSystem& fileSystem);//file system

		RenderingSystem::TextureCreateInfo loadTextureFile(const TheEngine::Platform::Path& path);
	};

}
