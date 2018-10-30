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

static Ticket_Mutex shader_factory_mutex;
void ShaderFactory::LoadShader(Effect* anEffect)
{
	std::string path = anEffect->m_FileName;
	size_t pos = path.rfind("/");
	std::string sub = path.substr(0, pos + 1);

	JSONReader reader(path);

	if (reader.DocumentHasMember("VertexShader"))
	{
		const JSONElement& el = reader.GetElement("VertexShader");
		LoadShader(reader.ReadElement(el, "file"), reader.ReadElement(el, "entrypoint"), eShaderType::VERTEX, anEffect);
	}

	if (reader.DocumentHasMember("PixelShader"))
	{
		const JSONElement& el = reader.GetElement("PixelShader");
		LoadShader(reader.ReadElement(el, "file"), reader.ReadElement(el, "entrypoint"), eShaderType::PIXEL, anEffect);
	}

	if (reader.DocumentHasMember("GeometryShader"))
	{
		const JSONElement& el = reader.GetElement("GeometryShader");
		LoadShader(reader.ReadElement(el, "file"), reader.ReadElement(el, "entrypoint"), eShaderType::GEOMETRY, anEffect);
	}

	if (reader.DocumentHasMember("HullShader"))
	{
		const JSONElement& el = reader.GetElement("HullShader");
		LoadShader(reader.ReadElement(el, "file"), reader.ReadElement(el, "entrypoint"), eShaderType::HULL, anEffect);
	}

	if (reader.DocumentHasMember("DomainShader"))
	{
		const JSONElement& el = reader.GetElement("DomainShader");
		LoadShader(reader.ReadElement(el, "file"), reader.ReadElement(el, "entrypoint"), eShaderType::DOMAINS, anEffect);
	}

	if (reader.DocumentHasMember("ComputeShader"))
	{
		const JSONElement& el = reader.GetElement("ComputeShader");
		LoadShader(reader.ReadElement(el, "file"), reader.ReadElement(el, "entrypoint"), eShaderType::COMPUTE, anEffect);
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

eShaderType CheckType(std::string type)
{
	if (type.find("vs") != type.npos)
		return eShaderType::VERTEX;
	else if (type.find("ps") != type.npos)
		return eShaderType::PIXEL;
	else if (type.find("gs") != type.npos)
		return eShaderType::GEOMETRY;
	else if (type.find("hs") != type.npos)
		return eShaderType::HULL;
	else if (type.find("ds") != type.npos)
		return eShaderType::DOMAINS;
	else if (type.find("cs") != type.npos)
		return eShaderType::COMPUTE;

	assert(false && "failed to find shader type");
	return NOF_TYPES;
}

void ShaderFactory::LoadShader(const std::string& filepath, const std::string& entrypoint, eShaderType type, Effect* effect)
{
	std::string full_path = "data/shaders/";
	full_path += filepath;
	full_path += entrypoint;

	u64 hash_key = cl::Hash(full_path.c_str());
	
	CompiledShader* shader = CreateShader(full_path, entrypoint, type);

	assert(shader && "Shader was null, failed to create the shader!");


	if (m_Shaders.find(hash_key) == m_Shaders.end())
		m_Shaders.emplace(hash_key, shader);
	
	effect->m_Shaders[type] = shader;

#ifndef FINAL
	full_path = "data/shaders/" + filepath;
	myFileWatchers[(s32)type]->WatchFileChangeWithDependencies(full_path, std::bind(&ShaderFactory::OnReload, this, std::placeholders::_1, entrypoint));
	ASSERT(effect, "Effect pointer was null");
	m_Shaders[hash_key]->RegisterReload(effect);
#endif
}

u64 ShaderFactory::LoadShader(const std::string& filepath, const std::string& entrypoint)
{
	std::string full_path = "data/shaders/";
	full_path += filepath;
	full_path += entrypoint;

	u64 hash_key = cl::Hash(full_path.c_str());

	if (m_Shaders.find(hash_key) != m_Shaders.end())
		return hash_key;


	const size_t pos = filepath.rfind(".");
	const eShaderType type = CheckType(filepath.substr(pos + 1));

	CompiledShader* shader = CreateShader(full_path, entrypoint, type);
	assert(shader && "Shader was null, failed to create the shader!");

	if (m_Shaders.find(hash_key) == m_Shaders.end())
		m_Shaders.emplace(hash_key, shader);
	
#ifndef FINAL
	full_path = "data/shaders/" + filepath;
	myFileWatchers[(s32)type]->WatchFileChangeWithDependencies(full_path, std::bind(&ShaderFactory::OnReload, this, std::placeholders::_1, entrypoint));
#endif
	return hash_key;
}

CompiledShader* ShaderFactory::CreateShader(const std::string& file_path, const std::string& entrypoint, eShaderType type)
{
 	ENGINE_LOG("Creating %s", file_path.c_str());

 	std::string shader_type(CheckType(type));

 	IShaderBlob* compiled_shader = CompileShader(file_path, entrypoint, shader_type);

	assert(compiled_shader && "IShaderBlob was null! No shader created!");

 	if (!compiled_shader)
 		return nullptr;
 
	void* shader = Engine::GetInstance()->CreateShader(compiled_shader, type, file_path.c_str());
	
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
	u64 hash_key = cl::Hash(fullpath.c_str());

	CU::GrowingArray<ShaderReload*> effect_container;
	CompiledShader* new_shader = nullptr;
	if (m_Shaders.find(hash_key) != m_Shaders.end())
	{
		CompiledShader* shader = m_Shaders[hash_key];
		new_shader = CreateShader(file_path, shader->m_Entrypoint, shader->m_Type);

		if (new_shader != nullptr)
		{
			const CU::GrowingArray<ShaderReload*>& objects = shader->m_Reloaders;
			for (ShaderReload* reloader : objects)
			{
				effect_container.Add(reloader);
			}

			delete shader;
			shader = nullptr;

			m_Shaders[hash_key] = new_shader;
		}
	}

	for (ShaderReload* reloader : effect_container)
	{
		reloader->Reload(m_Shaders[hash_key]);
	}
	m_Shaders[hash_key]->m_Reloaders = effect_container;

}
#endif
IShaderBlob* ShaderFactory::CompileShader(const std::string& file_path, const std::string& entrypoint, const std::string& shader_type)
{
	return Engine::GetAPI()->GetDevice().CompileShaderFromFile(file_path.c_str(), entrypoint.c_str(), shader_type.c_str());
}

CompiledShader* ShaderFactory::GetShader(u64 key) const
{
	auto it = m_Shaders.find(key);

	if (it != m_Shaders.end())
		return it->second;

	return nullptr;
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
