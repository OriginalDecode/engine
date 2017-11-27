#include "stdafx.h"
#include "VirtualFileSystem.h"

void VirtualFileSystem::Register(const std::string& path, const std::string& alias)
{
	m_RegisteredFolders.emplace(alias, path);
}

std::string VirtualFileSystem::GetFile(const std::string& path) const
{
	size_t pos = path.find("/");
	std::string alias = path.substr(0, pos);
	std::string file = path.substr(pos, path.npos);

	if ( m_RegisteredFolders.find(alias) != m_RegisteredFolders.end() )
	{
		return std::string(m_RegisteredFolders.at(alias) + file);
	}

	return path;
}

std::string VirtualFileSystem::GetFolder(const std::string& alias) const
{
	if (m_RegisteredFolders.find(alias) != m_RegisteredFolders.end())
	{
		return std::string(m_RegisteredFolders.at(alias) + "/");
	}
	DL_ASSERT("No folder found, did you forget to register the directory?");
	return "invalid_folder";
}
