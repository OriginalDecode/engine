#include "stdafx.h"
#include "ModelExporter.h"
#include "Model.h"
#include "Surface.h"
#include "IndexWrapper.h"
#include <string.h>
#include <stdio.h>

const char MODEL_EXT[5] = { 'L', 'P', 'M', 'F', '\0' }; //Linus Propriatary Model Format ¯\_(ツ)_/¯


ModelExporter::ModelExporter()
{
}

ModelExporter::~ModelExporter()
{
}

void ModelExporter::Export(Model* const pModel, const char* out)
{

#ifdef WIN32
	CreateDirectory("data/exported", NULL);
#endif

	std::stringstream ss;
	ss << out << "." << MODEL_EXT;

	char temp[100];
	sprintf_s(temp, 100, "%s", ss.str().c_str());
	FILE* hFile = fopen(temp, "wb");
	int err = errno;

	if (hFile != nullptr)
	{
		const char temp[5] = { 'R', 'O', 'O', 'T', '\0' };
		fwrite(&temp, sizeof(temp), 1, hFile);
		WriteBlock(pModel, hFile);
		fclose(hFile);
	}

}

void ModelExporter::WriteBlock(Model* const pModel, FILE* pOut)
{
	int byte_length = 0;
	
	WriteVertices((float*)pModel->GetVertexWrapper().GetData(), pModel->GetVertexWrapper().GetVertexCount(), pOut);
	WriteIndices((int*)pModel->GetIndexWrapper().GetData(), pModel->GetIndexWrapper().GetIndexCount(), pOut);

	const CU::GrowingArray<Surface*> surfaces = pModel->GetSurfaces();
	for (Surface* surface : surfaces)
	{
		WriteSurface(surface, pOut);
	}

	const CU::GrowingArray<Model*> children = pModel->GetChildModels();

	int child_count = children.Size();
	fwrite(&child_count, sizeof(int), 1, pOut);

	for (Model* child : children)
	{
		char temp[6] = { 'C', 'H', 'I', 'L', 'D', '\0' };
		fwrite(&temp, sizeof(temp), 1, pOut);
		WriteBlock(child, byte_length, pOut);
	}
}

void ModelExporter::WriteBlock(Model* const pModel, int& total_byte_length, FILE* pOut)
{
	WriteVertices((float*)pModel->GetVertexWrapper().GetData(), pModel->GetVertexWrapper().GetVertexCount(), pOut);
	WriteIndices((int*)pModel->GetIndexWrapper().GetData(), pModel->GetIndexWrapper().GetIndexCount(), pOut);

	const CU::GrowingArray<Surface*> surfaces = pModel->GetSurfaces();
	for (Surface* surface : surfaces)
	{
		WriteSurface(surface, pOut);
	}

	const CU::GrowingArray<Model*> children = pModel->GetChildModels();

	int child_count = children.Size();
	fwrite(&child_count, sizeof(int), 1, pOut);

	for (Model* child : children)
	{
		char temp[6] = { 'C', 'H', 'I', 'L', 'D', '\0' };
		fwrite(&temp, sizeof(temp), 1, pOut);
		WriteBlock(child, total_byte_length, pOut);
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
	
		const int len = strlen(texture_slots[rb.m_Slot]);
		fwrite(&len, sizeof(int), 1, pOut);
		fwrite(texture_slots[rb.m_Slot], len, 1, pOut);

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