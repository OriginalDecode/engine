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
}

class ModelExporter
{
public:
    ModelExporter();
    ~ModelExporter();

    void Export(Model* const pModel, const char* out);

private:

	void WriteBlock(Model* const pModel, Core::File* pOut);
	void WriteSurface(Surface* const pSurface, Core::File* pOut);
	void WriteVertices(float* const pVertices, int vertex_count, FILE* pOut);
	void WriteIndices(int* const pIndices, int indices_count, FILE* pOut);

	struct LPMFHeader
	{
		const char* fileExt{ nullptr };
		bool hasVertices = false;
		bool hasNormals = false;
		bool hasBiNormals = false;
		bool hasTangents = false;

	};
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

