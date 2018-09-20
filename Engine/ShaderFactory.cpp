#include "stdafx.h"
#include "ShaderFactory.h"
#include <Utilities.h>
#include <JSON/JSONReader.h>
#include <DataStructures/Hashmap/Hash.h>
#include <DataStructures/GrowingArray.h>
#include <Engine/Engine.h>
#include <Engine/IGraphicsDevice.h>

constexpr float SLEEP_TIME = 1000.f;
ShaderFactory::ShaderFactory()
{
#ifndef _FINAL 
	for (int i = 0; i < 6; i++)
	{
		FileWatcher* watcher = new FileWatcher();
		myFileWatchers.Insert(i, watcher);
	}
#endif
}

ShaderFactory::~ShaderFactory()
{
#ifndef _FINAL 
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
		const JSONElement& el = reader.GetElement("VertexShader");
		LoadShader(reader.ReadElement(el, "file"), reader.ReadElement(el, "entrypoint"), reader.OptionalReadElement(el, "sampler"), eShaderType::VERTEX, anEffect);
	}

	if (reader.DocumentHasMember("PixelShader"))
	{
		const JSONElement& el = reader.GetElement("PixelShader");
		LoadShader(reader.ReadElement(el, "file"), reader.ReadElement(el, "entrypoint"), reader.OptionalReadElement(el, "sampler"), eShaderType::PIXEL, anEffect);
	}

	if (reader.DocumentHasMember("GeometryShader"))
	{
		const JSONElement& el = reader.GetElement("GeometryShader");
		LoadShader(reader.ReadElement(el, "file"), reader.ReadElement(el, "entrypoint"), reader.OptionalReadElement(el, "sampler"), eShaderType::GEOMETRY, anEffect);
	}

	if (reader.DocumentHasMember("HullShader"))
	{
		const JSONElement& el = reader.GetElement("HullShader");
		LoadShader(reader.ReadElement(el, "file"), reader.ReadElement(el, "entrypoint"), reader.OptionalReadElement(el, "sampler"), eShaderType::HULL, anEffect);
	}

	if (reader.DocumentHasMember("DomainShader"))
	{
		const JSONElement& el = reader.GetElement("DomainShader");
		LoadShader(reader.ReadElement(el, "file"), reader.ReadElement(el, "entrypoint"), reader.OptionalReadElement(el, "sampler"), eShaderType::DOMAINS, anEffect);
	}

	if (reader.DocumentHasMember("ComputeShader"))
	{
		const JSONElement& el = reader.GetElement("ComputeShader");
		LoadShader(reader.ReadElement(el, "file"), reader.ReadElement(el, "entrypoint"), reader.OptionalReadElement(el, "sampler"), eShaderType::COMPUTE, anEffect);
	}
}

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

void ShaderFactory::LoadShader(const std::string& filepath, const std::string& entrypoint, const std::string& sampler, eShaderType type, Effect* effect)
{
	std::string full_path = Engine::GetInstance()->GetVFS().GetFolder("Shaders") + filepath;
	std::string to_hash(full_path + entrypoint);
	u64 hash_key = Hash(to_hash.c_str());
	
	if (m_Shaders.find(hash_key) == m_Shaders.end())
		m_Shaders.emplace(hash_key, CreateShader(full_path, entrypoint, type));

	
	//container->InsertShader(hash_key, )

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
	ASSERT(effect, "Effect pointer was null");
	m_Shaders[hash_key]->m_EffectPointers.Add(effect);
#endif
}

CompiledShader* ShaderFactory::CreateShader(const std::string& file_path, const std::string& entrypoint, eShaderType type)
{
 	ENGINE_LOG("Creating %s", file_path.c_str());

 	std::string shader_type(CheckType(type));

 	IShaderBlob* compiled_shader = CompileShader(file_path, entrypoint, shader_type);

 	if (!compiled_shader)
 		return nullptr;
 
	void* shader = Engine::GetInstance()->CreateShader(compiled_shader, type, file_path.c_str());

	auto container = Engine::GetInstance()->GetAssetsContainer();

	std::string file;

	size_t start = file_path.rfind("/");
	file = file_path.substr(start + 1);

	container->InsertShader(Hash(file.c_str()), shader);
	
 	return new CompiledShader(	compiled_shader,
								shader,
								type, 
								entrypoint.c_str());
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
		switch (new_shader->m_Type)
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
IShaderBlob* ShaderFactory::CompileShader(const std::string& file_path, const std::string& entrypoint, const std::string& shader_type)
{
	return Engine::GetAPI()->GetDevice().CompileShaderFromFile(file_path.c_str(), entrypoint.c_str(), shader_type.c_str());
}

void ShaderFactory::Update()
{
#ifndef _FINAL
	for (FileWatcher* watcher : myFileWatchers)
	{
		watcher->FlushChanges();
	}
#endif
}

CompiledShader::~CompiledShader()
{
	Engine::GetAPI()->ReleasePtr(m_Blob);
	Engine::GetAPI()->ReleasePtr(m_Shader);
}
