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

private:


	void WriteBlock(Model* const pModel, EMFFile* file);
	void WriteSurface(Surface* const pSurface, EMFFile* file);
	void WriteVertices(float* const pVertices, int vertex_count, FILE* file);
	void WriteIndices(int* const pIndices, int indices_count, FILE* file);

	void WriteBlock(Model* const pModel, FILE* pFile);
	void WriteSurface(Surface* const pSurface, FILE* pFile);
	//std::function<return(params...)>();

	using FFileWrite = std::function<void(const void* data, size_t elementSize, size_t elementCount)>;

	void WriteBlock(Model* const pModel, FFileWrite fnc);
	void WriteSurface(Surface* const pSurface, FFileWrite fnc);

	FILE* m_FileHandle = nullptr;
	std::ofstream m_OutStream;

	//void Write(const VertexData* pObj, size_t element_size, size_t element_count);
	
	template<typename T>
	void Write(const T* pObj, size_t element_size, size_t element_count);

};

template<typename T>
void ModelExporter::Write(const T* pObj, size_t element_size, size_t element_count)
{
	fwrite(pObj, element_size, element_count, m_FileHandle);

	//if (m_OutStream.is_open())
	//	m_OutStream << *pObj << "\n";
}

//template<>
//void ModelExporter::Write(const VertexData* pObj, size_t element_size, size_t element_count)
//{
//	fwrite(pObj, element_size, element_count, m_FileHandle);
//	if (m_OutStream.is_open())
//		m_OutStream << *pObj;
//}
