#include "stdafx.h"
#include "ModelExporter.h"
#include "Model.h"
#include "Surface.h"
#include "IndexWrapper.h"

#include <stdio.h>

const char MODEL_EXT[4] = { 'L', 'P', 'M', 'F'}; //Linus Propriatary Model Format ¯\_(ツ)_/¯


ModelExporter::ModelExporter()
{
}

ModelExporter::~ModelExporter()
{
}

void ModelExporter::Export(Model* const pModel, const char* out)
{
    char buffer[255];
    ZeroMemory(buffer, sizeof(buffer));

	std::string file = out;
	file += MODEL_EXT;
	const char* x = file.c_str();

	FILE* hFile = fopen(x, "wb");

	if (hFile != nullptr)
	{
		WriteBlock(pModel, hFile);
		fclose(hFile);
	}

}

void ModelExporter::WriteBlock(Model* const pModel, FILE* pOut)
{
	WriteVertices((float*)pModel->GetVertexWrapper().GetData(), pModel->GetVertexWrapper().GetVertexCount(), pOut);
	WriteIndices((int*)pModel->GetIndexWrapper().GetData(), pModel->GetIndexWrapper().GetIndexCount(), pOut);

	const CU::GrowingArray<Surface*> surfaces = pModel->GetSurfaces();
	for (Surface* surface : surfaces)
	{
		WriteSurface(surface, pOut);
	}

	const CU::GrowingArray<Model*> children = pModel->GetChildModels();

	int size = children.Size();
	fwrite(&size, sizeof(int), 1, pOut);

	for (Model* child : children)
	{
		WriteBlock(pModel, pOut);
	}
}

void ModelExporter::WriteSurface(Surface* const pSurface, FILE* pOut)
{
	const Material& material = pSurface->GetMaterial();
	for (const Material::ResourceBinding& rb : material.m_Resources)
	{
		const int size = rb.m_ResourceName.length();
		fwrite(&size, sizeof(int), 1, pOut);
		fwrite(rb.m_ResourceName.c_str(), size, 1, pOut);
	}
}

void ModelExporter::WriteVertices(float* const pVertices, int vertex_count, FILE* pOut)
{

	fwrite(&vertex_count, sizeof(int), 1, pOut);

	fwrite(pVertices, sizeof(float) * vertex_count, 1, pOut);

}

void ModelExporter::WriteIndices(int* const pIndices, int indices_count, FILE* pOut)
{

	fwrite(&indices_count, sizeof(int), 1, pOut);

	fwrite(pIndices, sizeof(int) * indices_count, 1, pOut);

}