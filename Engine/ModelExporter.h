#pragma once
#include "ModelImporter.h"

class VertexWrapper;
class IndexWrapper;
class Model;
class Surface;

namespace Core
{
	class File;
};

class ModelExporter
{
public:
	ModelExporter() = default;
	~ModelExporter() = default;

	void Export(Model* const pModel, const char* out);
	void SetOldFormat(bool oldFormat) { m_OldFormat = oldFormat; }
	bool GetFormat() const { return m_OldFormat; }
private:
	using FFileWrite = std::function<void(const void* data, size_t elementSize, size_t elementCount)>;

	void WriteBlock(Model* const pModel, FFileWrite fnc);
	void WriteSurface(Surface* const pSurface, FFileWrite fnc);

	FILE* m_FileHandle = nullptr;
	Core::File* m_File = nullptr;

	void Write(const void* pObj, size_t element_size, size_t element_count);
	bool m_OldFormat = false;

};
