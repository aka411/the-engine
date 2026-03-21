#pragma once
#include <string>

namespace TheEngine::Platform
{



	enum class MountPoint {
		ENGINE = 0, // Internal engine shaders, default textures, etc.
		GAME = 1,   // game assets, ---> game programmer defined
		USER = 2  // Save games, screenshots, config files, ---> game programmer defined
		//TEMP    // Cache or temporary files, might need it for game programmer
	};


	class Path
	{
		friend class FileSystem;

	private:

		const MountPoint m_mount;
		const std::string m_virtualPath;

	protected:

		// Static registry 
		static std::string s_MountPaths[3];


	public:

		
		Path(MountPoint mount, const std::string& relativePath);

		std::string getPhysicalPath() const;
		std::string getVirtualPath() const;


	};






}