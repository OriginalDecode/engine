#pragma once
#include <stdio.h>
#include <fstream>

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

	
};


template<typename T>
void _fwrite(const T* pObj, size_t element_size, size_t element_count, FILE* fileHandle, std::ofstream* stream = nullptr, std::string debug = "")
{
	fwrite(pObj, element_size, element_count, fileHandle);
#ifdef OUTPUT_MODEL_EXPORT
	if(stream)
		*stream << debug << *pObj << "\n";
#endif
}

