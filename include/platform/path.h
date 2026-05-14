#pragma once
#include <string>

namespace TheEngine::Platform
{



	enum class MountPoint
	{
		ENGINE = 0, // Internal engine shaders, default textures, etc.
		GAME = 1,   // game assets, ---> game programmer defined
		USER = 2,  // Save games, screenshots, config files, ---> game programmer defined
		//TEMP    // Cache or temporary files, might need it for game programmer

		COUNT = 3
	};


	class Path
	{
		friend class FileSystem;

	private:

	    MountPoint m_mount;
		std::string m_virtualPath;

	protected:



		static std::string s_MountPaths[(int)MountPoint::COUNT];

	
		static std::string getMountPrefix(MountPoint mount);

	public:

		static Path fromVFSString(const std::string& vfsPath);
		Path(MountPoint mount, const std::string& relativePath);

		std::string getPhysicalPath() const;
		std::string getVirtualPath() const;

		std::string getVFSString() const;
	};






}