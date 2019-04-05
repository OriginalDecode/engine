#include "stdafx.h"
#include "ModelExporter.h"
#include "Model.h"
#include "Surface.h"
#include "IndexWrapper.h"
#include <string.h>

#include "ModelImporter.h"
#include <CommonLib/File.h>

const char MODEL_EXT[5] = { 'L', 'P', 'M', 'F', '\0' }; //Linus Propriatary Model Format ¯\_(ツ)_/¯

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

	std::string fillename(out);
	fillename += ".";
	fillename += MODEL_EXT;

	Core::File file(fillename.c_str());

	WriteBlock(pModel, &file);



}
void ModelExporter::WriteBlock(Model* const pModel, Core::File* pOut)
{

	const int vtx_count = pModel->GetVertexWrapper().GetVertexCount();

	pOut->Write(&vtx_count, 1, sizeof(int));

	if (vtx_count > 0)
	{
		int8* data = pModel->GetVertexWrapper().GetData();
		for (size_t i = 0; i < vtx_count * sizeof(VertexData); i += sizeof(VertexData))
		{
			pOut->Write(&data[i], sizeof(VertexData), 1);
		}
	}


	const int idx_count = pModel->GetIndexWrapper().GetIndexCount();
	fwrite(&idx_count, sizeof(int), 1, pOut, &output, "index_count: ");
	g_ByteSize += sizeof(int);

	if (idx_count > 0)
	{
		int8* data = pModel->GetIndexWrapper().GetData();
		fwrite(data, sizeof(int), idx_count, pOut, &output, "index_data:");
		g_ByteSize += sizeof(int);
	}

	const CU::GrowingArray<Surface*> surfaces = pModel->GetSurfaces();
	const int surface_count = surfaces.Size();
	fwrite(&surface_count, sizeof(int), 1, pOut, &output, "surface_count: ");

	for (Surface* surface : surfaces)
	{
		WriteSurface(surface, pOut);
	}

	const CU::GrowingArray<Model*> children = pModel->GetChildModels();

	int child_count = children.Size();
	fwrite(&child_count, sizeof(int), 1, pOut, &output, "child_count: ");
	g_ByteSize += sizeof(int);

	for (Model* child : children)
	{
		WriteBlock(child, pOut);
	}
}

void ModelExporter::WriteSurface(Surface* const pSurface, Core::File* pOut)
{
	const Material& material = pSurface->GetMaterial();
	const int nof_bindings = material.m_Resources.Size();
	fwrite(&nof_bindings, sizeof(int), 1, pOut, &output, "material/nof_bindings: ");
	for (const Material::ResourceBinding& rb : material.m_Resources)
	{
		const int size = rb.m_ResourceName.length();
		fwrite(&size, sizeof(int), 1, pOut, &output, "material/filepath_length: "); //4
		g_ByteSize += sizeof(int);
		fwrite(rb.m_ResourceName.c_str(), size, 1, pOut, &output,"material/filepath: "); //len
		g_ByteSize += size;

		const int slot = rb.m_Slot;
		fwrite(&slot, sizeof(int), 1, pOut, &output, "material/bindslot: ");
		g_ByteSize += sizeof(int);

	}
}
