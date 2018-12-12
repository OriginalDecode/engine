#include "stdafx.h"
#include "ModelExporter.h"
#include "Model.h"
#include "Surface.h"
#include "IndexWrapper.h"
#include <string.h>
#include <stdio.h>
#include "ModelImporter.h"
const char MODEL_EXT[5] = { 'L', 'P', 'M', 'F', '\0' }; //Linus Propriatary Model Format ¯\_(ツ)_/¯


//make a readable file as well? for debugging
std::ofstream output;

void _fwrite(const VertexData* pObj, size_t element_size, size_t element_count, FILE* fileHandle, std::ofstream* stream = nullptr)
{
	fwrite(pObj, element_size, element_count, fileHandle);
#ifdef OUTPUT_MODEL_EXPORT
	if (stream)
		*stream << *pObj << "\n";
#endif
}


ModelExporter::ModelExporter()
{
}

ModelExporter::~ModelExporter()
{
}

int g_ByteSize = 0;


void ModelExporter::Export(Model* const pModel, const char* out)
{
	g_ByteSize = 0;
#ifdef WIN32
	CreateDirectory("data/exported", NULL);
#endif
	std::string readable(out);
	readable += ".txt";
	output.open(readable);
	if (output.is_open())
	{

	}


	std::stringstream ss;
	ss << out << "." << MODEL_EXT;

	char temp[100];
	sprintf_s(temp, 100, "%s", ss.str().c_str());
	FILE* hFile = fopen(temp, "wb");

	if (hFile != nullptr)
	{
		WriteBlock(pModel, hFile);
		fclose(hFile);
	}

	output.flush();
	output.close();


}
void ModelExporter::WriteBlock(Model* const pModel, FILE* pOut)
{

	const int vtx_count = pModel->GetVertexWrapper().GetVertexCount();
	_fwrite(&vtx_count, sizeof(int), 1, pOut, &output, "vertex_count: ");
	g_ByteSize += sizeof(int);
	if (vtx_count > 0)
	{
		int8* data = pModel->GetVertexWrapper().GetData();
		for (size_t i = 0; i < vtx_count * sizeof(VertexData); i += sizeof(VertexData))
		{
			fwrite(&data[i], sizeof(VertexData), 1, pOut);

			/*VertexData vtx_data;
			memcpy(&vtx_data, &data[i], sizeof(VertexData));
			output << vtx_data << "\n";
			
			
			g_ByteSize += sizeof(VertexData);*/
		}
	}


	const int idx_count = pModel->GetIndexWrapper().GetIndexCount();
	_fwrite(&idx_count, sizeof(int), 1, pOut, &output, "index_count: ");
	g_ByteSize += sizeof(int);

	if (idx_count > 0)
	{
		int8* data = pModel->GetIndexWrapper().GetData();
		_fwrite(data, sizeof(int), idx_count, pOut, &output, "index_data:");
		g_ByteSize += sizeof(int);
	}

	const CU::GrowingArray<Surface*> surfaces = pModel->GetSurfaces();
	const int surface_count = surfaces.Size();
	_fwrite(&surface_count, sizeof(int), 1, pOut, &output, "surface_count: ");

	for (Surface* surface : surfaces)
	{
		WriteSurface(surface, pOut);
	}

	const CU::GrowingArray<Model*> children = pModel->GetChildModels();

	int child_count = children.Size();
	_fwrite(&child_count, sizeof(int), 1, pOut, &output, "child_count: ");
	g_ByteSize += sizeof(int);

	for (Model* child : children)
	{
		WriteBlock(child, pOut);
	}
}

void ModelExporter::WriteSurface(Surface* const pSurface, FILE* pOut)
{
	const Material& material = pSurface->GetMaterial();
	const int nof_bindings = material.m_Resources.Size();
	_fwrite(&nof_bindings, sizeof(int), 1, pOut, &output, "material/nof_bindings: ");
	for (const Material::ResourceBinding& rb : material.m_Resources)
	{
		const int size = rb.m_ResourceName.length();
		_fwrite(&size, sizeof(int), 1, pOut, &output, "material/filepath_length: "); //4
		g_ByteSize += sizeof(int);
		_fwrite(rb.m_ResourceName.c_str(), size, 1, pOut, &output,"material/filepath: "); //len
		g_ByteSize += size;

		const int slot = rb.m_Slot;
		_fwrite(&slot, sizeof(int), 1, pOut, &output, "material/bindslot: ");
		g_ByteSize += sizeof(int);

	}
}
