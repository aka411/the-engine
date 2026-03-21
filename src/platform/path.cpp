#include <platform/path.h>


namespace TheEngine::Platform
{
	std::string Path::s_MountPaths[3];


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
		return m_virtualPath;
	}






}