#include <platform/path.h>
#include <assert.h>


namespace TheEngine::Platform
{
	std::string Path::s_MountPaths[3];

	std::string Path::getMountPrefix(MountPoint mount) 
	{
		switch (mount)
		{
		case MountPoint::ENGINE: return "ENGINE";
		case MountPoint::GAME:   return "GAME";
		case MountPoint::USER:   return "USER";
		default:
		{
			assert(false && "Unkown Mount Point");
			return "UNKNOWN";
		}
		}
	}

	Path::Path(MountPoint mount, const std::string& relativePath) :
		m_mount(mount),
		m_virtualPath(relativePath)
	{
	

	}

	std::string Path::getPhysicalPath() const
	{
		return s_MountPaths[int(m_mount)] + m_virtualPath;
	}


	std::string Path::getVirtualPath() const
	{
		//need to include mount point also
		return m_virtualPath;
	}


	std::string Path::getVFSString() const 
	{
		return getMountPrefix(m_mount) + ":/" + m_virtualPath;
	}

	Path Path::fromVFSString(const std::string& vfsPath) 
	{
		size_t separator = vfsPath.find(":/");
		if (separator == std::string::npos) {
			
			assert(false && "Invalid Mount Point in VFS Path");
			return Path(MountPoint::GAME, vfsPath);
		}

		std::string prefix = vfsPath.substr(0, separator);
		std::string relPath = vfsPath.substr(separator + 2); // skip the ":/"

		MountPoint mount = MountPoint::GAME;
		if (prefix == "ENGINE") mount = MountPoint::ENGINE;
		else if (prefix == "USER") mount = MountPoint::USER;

		return Path(mount, relPath);
	}



}