#include "stdafx.h"
#include "ModelExporter.h"
#include "Model.h"
#include "Surface.h"
#include "IndexWrapper.h"
#include <string.h>

#include "ModelImporter.h"
#include <CommonLib/File.h>


std::ofstream output;
void _fwrite(const VertexData* pObj, size_t element_size, size_t element_count, FILE* fileHandle, std::ofstream* stream = nullptr)
{
	fwrite(pObj, element_size, element_count, fileHandle);
#ifdef OUTPUT_MODEL_EXPORT
	if (stream)
		*stream << *pObj << "\n";
#endif
}

const bool old = false;


void ModelExporter::Export(Model* const pModel, const char* out)
{
#ifdef WIN32
	CreateDirectory("data/exported", NULL);
#endif
	std::string fillename(out);
	fillename += ".EMF";

	EMFFile file = {};
	file.m_File = new Core::File(fillename.c_str(), Core::FileMode::WRITE_FILE);

	file.m_File->Write(&file.m_Header, sizeof(EMFHeader), 1);

	WriteBlock(pModel, &file);


}
void ModelExporter::WriteBlock(Model* const pModel, EMFFile* file)
{
	Core::File* fileBuffer = file->m_File;

	const int vtx_count = pModel->GetVertexWrapper().GetVertexCount();
	fileBuffer->Write(&vtx_count, sizeof(int), 1);

	if (vtx_count > 0)
	{
		int8* data = pModel->GetVertexWrapper().GetData();
		for (size_t i = 0; i < vtx_count * sizeof(VertexData); i += sizeof(VertexData))
		{
			fileBuffer->Write(&data[i], sizeof(VertexData), 1);
		}
	}

	const int idx_count = pModel->GetIndexWrapper().GetIndexCount();
	fileBuffer->Write(&idx_count, sizeof(int), 1);

	if (idx_count > 0)
	{
		int8* data = pModel->GetIndexWrapper().GetData();
		fileBuffer->Write(data, sizeof(int), idx_count);
	}

	const CU::GrowingArray<Surface*> surfaces = pModel->GetSurfaces();
	const int surface_count = surfaces.Size();
	fileBuffer->Write(&surface_count, sizeof(int), 1);

	for (Surface* surface : surfaces)
	{
		WriteSurface(surface, file);
	}

	const CU::GrowingArray<Model*> children = pModel->GetChildModels();

	int child_count = children.Size();
	fileBuffer->Write(&child_count, sizeof(int), 1);

	for (Model* child : children)
	{
		WriteBlock(child, file);
	}
}

void ModelExporter::WriteSurface(Surface* const pSurface, EMFFile* file)
{
	Core::File* fileBuffer = file->m_File;
	const Material& material = pSurface->GetMaterial();
	const int nof_bindings = material.m_Resources.Size();
	fileBuffer->Write(&nof_bindings, sizeof(int), 1);
	for (const Material::ResourceBinding& rb : material.m_Resources)
	{
		const int size = rb.m_ResourceName.length();
		fileBuffer->Write(&size, sizeof(int), 1); //4
		fileBuffer->Write(rb.m_ResourceName.c_str(), size, 1); //len

		const int slot = rb.m_Slot;
		fileBuffer->Write(&slot, sizeof(int), 1);

	}
}

EMFFile::~EMFFile()
{
	delete m_File;
	m_File = nullptr;
}
