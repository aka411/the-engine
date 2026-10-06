#pragma once
#include <string>

namespace TheEngine::Platform
{



	enum class MountPoint
	{
		APP   = 0,  // shaders, default textures, game assets etc. ---> READ ONLY

		USER  = 1,  // Save games, screenshots,read write. ---->READ WRITE
		TEMP  = 2,  // Cache or temporary files.  ---->READ WRITE

		COUNT = 3,
		UNKNOWN
	};
	

	class Path
	{

	private:

		MountPoint m_mountPoint{ MountPoint::UNKNOWN };
		std::string m_pathString{};

	public:

		Path() = default;
		Path(const MountPoint mountPoint, std::string_view relativePath);


		const MountPoint getMountPoint() const;
		const std::string_view getRelativePath() const;


	};


}