#pragma once
#include "engine_shared.h"
#include <map>
class VirtualFileSystem
{
public:
	VirtualFileSystem() = default;
	void Register(const std::string& path, const std::string& alias);
	std::string GetFile(const std::string& path) const;
	std::string GetFolder(const std::string& alias) const;
private:
	std::map<std::string, std::string> m_RegisteredFolders;

};

typedef VirtualFileSystem VFS;