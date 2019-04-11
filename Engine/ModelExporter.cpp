#include "stdafx.h"
#include "ModelExporter.h"


#include "Model.h"
#include "Surface.h"
#include "IndexWrapper.h"

#include <CommonLib/File.h>

#include <string.h>


void ModelExporter::Export(Model* const pModel, const char* out)
{
#ifdef WIN32
	CreateDirectory("data/exported", nullptr);
#endif

	std::string filename(out);
	if (!m_OldFormat)
	{
		filename += ".emf";
		m_File = new Core::File(filename.c_str(), Core::FileMode::WRITE_FILE);
		WriteBlock(pModel, [&](const void* object, size_t szElem, size_t nofElem) { m_File->Write(object, szElem, nofElem); });
		delete m_File;
		m_File = nullptr;
	}
	else
	{
		filename += ".oemf";
		if (m_FileHandle = fopen(filename.c_str(), "wb"))
		{
			WriteBlock(pModel, [&](const void* object, size_t szElem, size_t nofElem) { Write(object, szElem, nofElem); });
			fclose(m_FileHandle);
		}
	}
}

void ModelExporter::WriteBlock(Model* const pModel, FFileWrite fnc)
{
	const int vtx_count = pModel->GetVertexWrapper().GetVertexCount();
	fnc(&vtx_count, sizeof(int), 1);

	if (vtx_count > 0)
	{
		int8* data = pModel->GetVertexWrapper().GetData();
		fnc(data, sizeof(VertexData), vtx_count);
	}

	const int idx_count = pModel->GetIndexWrapper().GetIndexCount();
	fnc(&idx_count, sizeof(int), 1);
	if (idx_count > 0)
	{
		int8* data = pModel->GetIndexWrapper().GetData();
		fnc(data, sizeof(int), idx_count);
	}

	const CU::GrowingArray<Surface*> surfaces = pModel->GetSurfaces();
	const int surface_count = surfaces.Size();
	fnc(&surface_count, sizeof(int), 1);

	for (Surface* surface : surfaces)
	{
		WriteSurface(surface, fnc);
	}

	const CU::GrowingArray<Model*> children = pModel->GetChildModels();

	int child_count = children.Size();
	fnc(&child_count, sizeof(int), 1);

	for (Model* child : children)
	{
		WriteBlock(child, fnc);
	}
}

void ModelExporter::WriteSurface(Surface* const pSurface, FFileWrite fnc)
{
	const Material& material = pSurface->GetMaterial();
	const int nof_bindings = material.m_Resources.Size();
	fnc(&nof_bindings, sizeof(int), 1);

	for (const Material::ResourceBinding& rb : material.m_Resources)
	{
		const int size = (int)rb.m_ResourceName.length();
		fnc(&size, sizeof(int), 1); //4
		fnc(rb.m_ResourceName.c_str(), size, 1); //len


		const int slot = rb.m_Slot;
		fnc(&slot, sizeof(int), 1);
	}
}

void ModelExporter::Write(const void* pObj, size_t element_size, size_t element_count)
{
	fwrite(pObj, element_size, element_count, m_FileHandle);
}
