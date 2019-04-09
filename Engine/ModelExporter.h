#pragma once
#include <stdio.h>
#include <fstream>
#include "ModelImporter.h"

class VertexWrapper;
class IndexWrapper;
class Model;
class Surface;
/*
    Export models to support the correct formatting we use in the engine.
    This should preferably be converted into a tool that can be used for Blender 2.7+ and Autodesk Maya / Autodesk Maya LT
*/

namespace Core
{
	class File;
};

struct EMFHeader
{
	const char fileExt[4]{ 'E', 'M', 'F', '\0' };
	bool hasVertices = true;
	bool hasNormals = true;
	bool hasBiNormals = true;
	bool hasTangents = true;
	bool hasTexcoord = true;
};

struct EMFFile
{
	~EMFFile();
	EMFHeader m_Header;
	Core::File* m_File = nullptr;
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
	//std::function<return(params...)>();

	using FFileWrite = std::function<void(const void* data, size_t elementSize, size_t elementCount)>;

	void WriteBlock(Model* const pModel, FFileWrite fnc);
	void WriteSurface(Surface* const pSurface, FFileWrite fnc);

	FILE* m_FileHandle = nullptr;
	std::ofstream m_OutStream;
	
	void Write(const void* pObj, size_t element_size, size_t element_count);
	bool m_OldFormat = false;
};
