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
class ModelExporter
{
public:
    ModelExporter();
    ~ModelExporter();

    void Export(Model* const pModel, const char* out);

private:

	void WriteBlock(Model* const pModel, FILE* pOut);
	void WriteSurface(Surface* const pSurface, FILE* pOut);
	void WriteVertices(float* const pVertices, int vertex_count, FILE* pOut);
	void WriteIndices(int* const pIndices, int indices_count, FILE* pOut);


};


template<typename T>
void _fwrite(const T* pObj, size_t element_size, size_t element_count, FILE* fileHandle, std::ofstream* stream = nullptr, std::string debug = "")
{
	fwrite(pObj, element_size, element_count, fileHandle);
	if(stream)
		*stream << debug << *pObj << "\n";
}

