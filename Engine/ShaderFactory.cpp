#include "stdafx.h"
#include "ShaderFactory.h"
#include <d3dcompiler.h>
#include <Utilities.h>
#include <JSON/JSONReader.h>
#include <DataStructures/Hashmap/Hash.h>
#include <DataStructures/GrowingArray.h>
#define SLEEP_TIME 1000

ShaderFactory::ShaderFactory()
{
#ifndef FINAL 
	for (int i = 0; i < 6; i++)
	{
		FileWatcher* watcher = new FileWatcher();
		myFileWatchers.Insert(i, watcher);
	}
#endif
}

ShaderFactory::~ShaderFactory()
{
#ifndef FINAL 
	myFileWatchers.DeleteAll();
#endif
	DELETE_MAP(m_Shaders);
}

void ShaderFactory::LoadShader(Effect* anEffect)
{
	std::string path = anEffect->m_FileName;
	s32 pos = path.rfind("/");
	std::string sub = path.substr(0, pos + 1);

	JSONReader reader(path);

	if (reader.DocumentHasMember("VertexShader"))
	{
		const JSONElement& el0 = reader.GetElement("VertexShader");
		LoadShader(reader.ReadElement(el0, "file"), reader.ReadElement(el0, "entrypoint"), eShaderType::VERTEX, anEffect);
	}
	if (reader.DocumentHasMember("PixelShader"))
	{
		const JSONElement& el1 = reader.GetElement("PixelShader");
		LoadShader(reader.ReadElement(el1, "file"), reader.ReadElement(el1, "entrypoint"), eShaderType::PIXEL, anEffect);
	}

	if (reader.DocumentHasMember("GeometryShader"))
	{
		const JSONElement& el2 = reader.GetElement("GeometryShader");
		LoadShader(reader.ReadElement(el2, "file"), reader.ReadElement(el2, "entrypoint"), eShaderType::GEOMETRY, anEffect);
	}

	if (reader.DocumentHasMember("HullShader"))
	{
		const JSONElement& el3 = reader.GetElement("HullShader");
		LoadShader(reader.ReadElement(el3, "file"), reader.ReadElement(el3, "entrypoint"), eShaderType::HULL, anEffect);
	}

	if (reader.DocumentHasMember("DomainShader"))
	{
		const JSONElement& el4 = reader.GetElement("DomainShader");
		LoadShader(reader.ReadElement(el4, "file"), reader.ReadElement(el4, "entrypoint"), eShaderType::DOMAINS, anEffect);
	}

	if (reader.DocumentHasMember("ComputeShader"))
	{
		const JSONElement& el5 = reader.GetElement("ComputeShader");
		LoadShader(reader.ReadElement(el5, "file"), reader.ReadElement(el5, "entrypoint"), eShaderType::COMPUTE, anEffect);
	}
}


// std::string CheckType(const std::string& file_path)
// {
// 	if (file_path.find(vertex_shader) != file_path.npos)
// 		return vertex_shader;
// 	else if (file_path.find(pixel_shader) != file_path.npos)
// 		return pixel_shader;
// 	else if (file_path.find(geometry_shader) != file_path.npos)
// 		return geometry_shader;
// 	else if (file_path.find(hull_shader) != file_path.npos)
// 		return hull_shader;
// 	else if (file_path.find(domain_shader) != file_path.npos)
// 		return domain_shader;
// 	else if (file_path.find(compute_shader) != file_path.npos)
// 		return compute_shader;
// 
// 	return "NO_TYPE";
// }

char* CheckType(eShaderType type)
{
	switch (type)
	{
		case eShaderType::VERTEX:
			return "vs";
		case eShaderType::PIXEL:
			return "ps";
		case eShaderType::GEOMETRY:
			return "gs";
		case eShaderType::HULL:
			return "hs";
		case eShaderType::DOMAINS:
			return "ds";
		case eShaderType::COMPUTE:
			return "cs";
	}
	return "invalid";
}



std::string ToLower(const std::string& str)
{
	std::string to_return = str;
	for (s32 i = 0; i < to_return.length(); i++)
	{
		to_return[i] = tolower(str[i]);
	}
	return to_return;
}

void ShaderFactory::LoadShader(const std::string& filepath, const std::string& entrypoint, eShaderType type, Effect* effect)
{
	std::string full_path = Engine::GetInstance()->GetVFS().GetFolder("Shaders") + filepath;
	std::string to_hash(full_path + entrypoint);
	u64 hash_key = Hash(to_hash.c_str());

	if (m_Shaders.find(hash_key) == m_Shaders.end())
		m_Shaders.emplace(hash_key, CreateShader(full_path, entrypoint, type));

	switch (type)
	{
		case eShaderType::VERTEX:
		{
			effect->m_VertexShader = m_Shaders[hash_key];
		} break;
		case eShaderType::PIXEL:
		{
			effect->m_PixelShader = m_Shaders[hash_key];
		} break;
		case eShaderType::GEOMETRY:
		{
			effect->m_GeometryShader = m_Shaders[hash_key];
		} break;
		case eShaderType::HULL:
		{
			effect->m_HullShader = m_Shaders[hash_key];
		} break;
		case eShaderType::DOMAINS:
		{
			effect->m_DomainShader = m_Shaders[hash_key];
		} break;
		case eShaderType::COMPUTE:
		{
			effect->m_ComputeShader = m_Shaders[hash_key];
		} break;

		default:
			DL_ASSERT("No valid shader type");
			break;
	}

#ifndef FINAL
	myFileWatchers[(s32)type]->WatchFileChangeWithDependencies(full_path, std::bind(&ShaderFactory::OnReload, this, std::placeholders::_1, entrypoint));
#endif

	DL_ASSERT_EXP(effect, "Effect pointer was null");
	m_Shaders[hash_key]->m_EffectPointers.Add(effect);
}

CompiledShader* ShaderFactory::CreateShader(const std::string& file_path, const std::string& entrypoint, eShaderType type, const std::string& feature_level /*= "_5_0"*/)
{
	ENGINE_LOG("Creating %s", file_path.c_str());

	std::string shader_type(CheckType(type));
	ID3D10Blob* compiled_shader = CompileShader(file_path, entrypoint, shader_type + feature_level);
	if (!compiled_shader)
		return nullptr;

	CompiledShader* new_shader = new CompiledShader;
	new_shader->m_Shader = Engine::GetInstance()->CreateShader(compiled_shader, type, file_path);
	new_shader->m_Blob = compiled_shader;
	new_shader->m_CompiledShader = compiled_shader->GetBufferPointer();
	//new_shader->m_haderSize = compiled_shader->GetBufferSize();
	new_shader->m_Entrypoint = entrypoint;
	new_shader->m_Type = type;
	return new_shader;
}

#ifndef FINAL 
void ShaderFactory::OnReload(const std::string& file_path, const std::string& entrypoint)
{
	Sleep(SLEEP_TIME);
	std::string fullpath = file_path + entrypoint;
	u64 hash_key = Hash(fullpath.c_str());

	CU::GrowingArray<Effect*> effect_container;
	CompiledShader* new_shader = nullptr;
	if (m_Shaders.find(hash_key) != m_Shaders.end())
	{
		CompiledShader* shader = m_Shaders[hash_key];
		new_shader = CreateShader(file_path, shader->m_Entrypoint, shader->m_Type);
		if (new_shader != nullptr)
		{
			const CU::GrowingArray<Effect*>& effects = m_Shaders[hash_key]->m_EffectPointers;
			for (Effect* effect : effects)
			{
				effect_container.Add(effect);
			}

			delete m_Shaders[hash_key];
			m_Shaders[hash_key] = nullptr;

			m_Shaders[hash_key] = new_shader;
		}
	}


	for (Effect* effect : effect_container)
	{
		switch (new_shader->type)
		{
			case eShaderType::VERTEX:
			{
				effect->m_VertexShader = m_Shaders[hash_key];
			} break;
			case eShaderType::PIXEL:
			{
				effect->m_PixelShader = m_Shaders[hash_key];
			} break;
			case eShaderType::GEOMETRY:
			{
				effect->m_GeometryShader = m_Shaders[hash_key];
			} break;
			case eShaderType::HULL:
			{
				effect->m_HullShader = m_Shaders[hash_key];
			} break;
			case eShaderType::DOMAINS:
			{
				effect->m_DomainShader = m_Shaders[hash_key];
			} break;
			case eShaderType::COMPUTE:
			{
				effect->m_ComputeShader = m_Shaders[hash_key];
			} break;

			default:
				DL_ASSERT("No valid shader type");
				break;
		}
		m_Shaders[hash_key]->m_EffectPointers.Add(effect);
	}
}
#endif
void* ShaderFactory::CompileShader(const std::string& file_path, const std::string& entrypoint, const std::string& feature_level)
{
	unsigned int shaderFlag = D3D10_SHADER_ENABLE_STRICTNESS;
#ifdef _DEBUG 
	shaderFlag |= D3D10_SHADER_DEBUG;
	shaderFlag |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

	ID3D10Blob* compiledShader = nullptr;
	ID3D10Blob* compilationMessage = nullptr;


	HRESULT hr = Engine::GetInstance()->CompileShaderFromFile(file_path, entrypoint, feature_level, shaderFlag, compiledShader, compilationMessage);
	if (compilationMessage != nullptr)
	{
		DL_WARNING("%s has generated warnings!", file_path.c_str());
		DL_WARNING("\n%s", (char*)compilationMessage->GetBufferPointer());
	}
	//Engine::GetInstance()->GetAPI()->HandleErrors(hr, "Failed to compile shader!");
	return compiledShader;
}

void ShaderFactory::Update()
{
	for (FileWatcher* watcher : myFileWatchers)
	{
		watcher->FlushChanges();
	}
}

CompiledShader::~CompiledShader()
{
	ID3D10Blob* blob = static_cast<ID3D10Blob*>(m_Blob);
	SAFE_RELEASE(blob);
	
	if (!m_Shader)
		return; 
	IUnknown* unknown_pointer = static_cast<IUnknown*>(m_Shader);
	unknown_pointer->Release();
	unknown_pointer = nullptr;
	m_Shader = nullptr;
	m_CompiledShader = nullptr;
}
