#include "stdafx.h"
#include "ModelExporter.h"
#include "Model.h"
#include "Surface.h"
#include "IndexWrapper.h"
#include <string.h>

#include <CommonLib/File.h>



const bool old = true;


void ModelExporter::Export(Model* const pModel, const char* out)
{
#ifdef WIN32
	CreateDirectory("data/exported", NULL);
#endif
	std::string _export(out);
	_export += "-export.txt";
	m_OutStream.open(_export.c_str());
	std::string filename(out);
	if (!old)
	{
		filename += ".emf";

		EMFFile file = {};
		file.m_File = new Core::File(filename.c_str(), Core::FileMode::WRITE_FILE);
		file.m_File->Write(&file.m_Header, sizeof(EMFHeader), 1);
		WriteBlock(pModel, [&](const void* object, size_t szElem, size_t nofElem) { file.m_File->Write(object, szElem, nofElem); });
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

	m_OutStream.flush();
	m_OutStream.close();
}

void ModelExporter::WriteBlock(Model* const pModel, FFileWrite fnc)
{
	const int vtx_count = pModel->GetVertexWrapper().GetVertexCount();
	fnc(&vtx_count, sizeof(int), 1);

	m_OutStream << "vertex count : " << vtx_count << "\n";
	if (vtx_count > 0)
	{
		int8* data = pModel->GetVertexWrapper().GetData();
		for (size_t i = 0; i < vtx_count * sizeof(VertexData); i += sizeof(VertexData))
		{
			fnc(&data[i], sizeof(VertexData), 1);

			VertexData _data = {};
			memcpy(&_data, &data[i], sizeof(VertexData));
			m_OutStream << _data;
		}
	}

	const int idx_count = pModel->GetIndexWrapper().GetIndexCount();
	fnc(&idx_count, sizeof(int), 1);
	m_OutStream << "index count : " << idx_count << "\n";
	if (idx_count > 0)
	{
		int8* data = pModel->GetIndexWrapper().GetData();
		fnc(data, sizeof(int), idx_count);

		for (int i = 0, s = 0; i < idx_count * sizeof(int); i += sizeof(int), s++)
		{
			if (s == 3)
			{
				s = 0;
				m_OutStream << "\n";
			}

			int _data = 0;
			memcpy(&_data, &data[i], sizeof(int));
			m_OutStream << _data << " ";
		}
	}

	const CU::GrowingArray<Surface*> surfaces = pModel->GetSurfaces();
	const int surface_count = surfaces.Size();
	fnc(&surface_count, sizeof(int), 1);
	m_OutStream << "surface_count : " << surface_count << "\n";

	for (Surface* surface : surfaces)
	{
		WriteSurface(surface, fnc);
	}

	const CU::GrowingArray<Model*> children = pModel->GetChildModels();

	int child_count = children.Size();
	fnc(&child_count, sizeof(int), 1);
	m_OutStream << "child_count : " << child_count << "\n";

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
	m_OutStream << "nof_bindings : " << nof_bindings << "\n";

	for (const Material::ResourceBinding& rb : material.m_Resources)
	{
		const size_t size = rb.m_ResourceName.length();
		fnc(&size, sizeof(size_t), 1); //4
		fnc(rb.m_ResourceName.c_str(), size, 1); //len

		m_OutStream << "resourcename-length" << size << "\n";
		m_OutStream << rb.m_ResourceName << "\n";

		const size_t slot = rb.m_Slot;
		fnc(&slot, sizeof(size_t), 1);

		m_OutStream << "slot : " << slot << "\n";
	}
}

EMFFile::~EMFFile()
{
	delete m_File;
	m_File = nullptr;
}
