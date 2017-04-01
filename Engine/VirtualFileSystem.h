#pragma once
#include "snowblind_shared.h"
#include <map>
class VirtualFileSystem
{
public:
	VirtualFileSystem() = default;
	void Register(const std::string& path, const std::string& alias);
	std::string GetDirectory(const std::string& path) const;

private:
	std::map<std::string, std::string> m_RegisteredFolders;

};

typedef VirtualFileSystem VFS;