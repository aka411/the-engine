#include <platform/file-system/path.h>
#include <assert.h>


namespace TheEngine::Platform
{


	Path::Path(const MountPoint mountPoint, std::string_view relativePath)
	{
		m_pathString = relativePath;
		m_mountPoint = mountPoint;
	}


	const MountPoint Path::getMountPoint() const
	{
		return m_mountPoint;
	}

	const std::string_view Path::getRelativePath() const
	{
		return m_pathString;
	}


}